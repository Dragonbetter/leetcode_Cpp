// 
#define TUEOBJFUSN_TRACKABLELIST_U16VALIDTRACKABLES_MAX \
    (TUE_PRV_FUSION_TRACKABLE_LIST_SIZE)
    
#define TUEOBJFUSN_TRACKABLE_U16LIFESPAN_FREE_SLOT (0u)

uint32 lkfTrackManagement_predictInternalTrkbl(float32 const f32PredictionDt) 
{
    uint32 u32Success = TUEOBJFUSN_ERROR_NOERROR;
    uint16 u16i;

    P2VAR(TueObjFusn_TrackableType, AUTOMATIC, ObjFusn_VAR_NOINIT) pCurrTrkbl;

    #if (TUE_PRV_RUNTIME_ERROR_CHECK == STD_ON)
        if (FLT_ZERO > f32PredictionDt) {
            u32Success = TUEOBJFUSN_ERROR_INVALID_INPUT;
            (void)tue_prv_error_management_addError(
                u32Success, TUEOBJFUSN_AAU_LKF_TRACKMANAGEMENT,
                TUEOBJFUSN_AAU_LKF_TRACK_MANAGEMENT_PREDICT_INTERNAL_TRKBL);
        } else
    #endif
        {
            for (u16i = 0u; u16i < TUEOBJFUSN_TRACKABLELIST_U16VALIDTRACKABLES_MAX;
                u16i++) 
            {
                if (TUEOBJFUSN_TRACKABLE_U16LIFESPAN_FREE_SLOT <
                    TRK_LIST.aTrackable[u16i].u16Lifespan) 
                {
                    pCurrTrkbl = &(TRK_LIST.aTrackable[u16i]);

                    u32Success |= LKF_DoPredict(pCurrTrkbl, f32PredictionDt);
                    u32Success |= LKF_CoordinatedTurn_DoPredict(pCurrTrkbl, f32PredictionDt);

                    /** Calculate gain if desired */
                    #if TUE_PRV_GAIN_ESTIMATION_ENABLE_GAIN_CALCULATION == STD_ON
                        u32Success |= gain_prediction(pCurrTrkbl, f32PredictionDt);
                    #endif
                } 
                else {
                    /* MISRA */
                }
            }
        }

    /** Reduce Track list age by prediction time as track list now is "younger"
     */
    if (TUEOBJFUSN_ERROR_NOERROR == u32Success) {
        TRK_LIST.f32MeasurementLatency -= f32PredictionDt;
    } else {
        /* MISRA */
    }

    return u32Success;
}



// ====================>
uint32 LKF_DoPredict(CONSTP2VAR(TueObjFusn_TrackableType,
                                AUTOMATIC,
                                ObjFusn_VAR_NOINIT) pTrkbl,
                     const float32 f32dT) {
    /** function process: */
    VAR(stf32SymMatrix_t, ObjFusn_VAR_NOINIT) matQ;
    uint32 u32Success = TUEOBJFUSN_ERROR_NOERROR;
    const float32 f32dTSquared = FLT_ONE_HALF * f32dT * f32dT;
    const float32 f32dTpow2 = f32dT * f32dT;
    const float32 f32dTpow3 = f32dTpow2 * f32dT;
    const float32 f32dTpow4 = f32dTpow3 * f32dT;
    const float32 f32dTpow5 = f32dTpow4 * f32dT;
    float32 f32PrefactorNoiseX; /* = q for x direction in equation on top */
    float32 f32PrefactorNoiseY; /* = q for y direction in equation on top */

    matQ.u16Size = pTrkbl->vecX.nRows;

    #if (TUE_PRV_RUNTIME_ERROR_CHECK == STD_ON)
        if (pTrkbl->vecX.nRows < TRACKABLE_VELY) {
            u32Success = TUEOBJFUSN_ERROR_INPUT_NOT_ENOUGH_STATES;
            (void)tue_prv_error_management_addError(
                TUEOBJFUSN_ERROR_INPUT_NOT_ENOUGH_STATES, TUEOBJFUSN_AAU_LKF,
                TUEOBJFUSN_AAU_LKF_COORDINATED_TURN_DO_PREDICT);
        } else
    #endif
    {
        if (((pTrkbl->u16Class &
              TUEOBJFUSN_TRACKABLE_U16CLASS_VEHICLE_UNKNOWN) > 0u) ||
            (pTrkbl->u16Class ==
             TUEOBJFUSN_TRACKABLE_U16CLASS_2WHEELER_MOTORCYCLE)) {
            f32PrefactorNoiseX = Fusion_get_f32VehicleVarianceInXForQ();
            f32PrefactorNoiseY = Fusion_get_f32VehicleVarianceInYForQ();
        } else {
            f32PrefactorNoiseX = Fusion_get_f32PedestrianVarianceInXForQ();
            f32PrefactorNoiseY = Fusion_get_f32PedestrianVarianceInYForQ();
        }

        if (TRACKABLE_ACCY < pTrkbl->vecX.nRows) {
            pTrkbl->vecX.data[TRACKABLE_POSX] =
                (pTrkbl->vecX.data[TRACKABLE_POSX] +
                 (f32dT * pTrkbl->vecX.data[TRACKABLE_VELX])) +
                (f32dTSquared * pTrkbl->vecX.data[TRACKABLE_ACCX]);
            pTrkbl->vecX.data[TRACKABLE_POSY] =
                (pTrkbl->vecX.data[TRACKABLE_POSY] +
                 (f32dT * pTrkbl->vecX.data[TRACKABLE_VELY])) +
                (f32dTSquared * pTrkbl->vecX.data[TRACKABLE_ACCY]);
            pTrkbl->vecX.data[TRACKABLE_VELX] =
                pTrkbl->vecX.data[TRACKABLE_VELX] +
                (f32dT * pTrkbl->vecX.data[TRACKABLE_ACCX]);
            pTrkbl->vecX.data[TRACKABLE_VELY] =
                pTrkbl->vecX.data[TRACKABLE_VELY] +
                (f32dT * pTrkbl->vecX.data[TRACKABLE_ACCY]);

            /* Init Q Matrix for vx-px, vy-py, ax-px ay-py,vx-ax,vy-ay */
            /* PRQA S 3121 1 */ /* Mathematical formula */
            matQ.data[TRACKABLE_INDEX_VARIANCE_POSX] =
                0.05f * f32PrefactorNoiseX * f32dTpow5;
            /* PRQA S 3120 */   /* Mathematical formula */
            /* PRQA S 3121 1 */ /* Mathematical formula */
            matQ.data[TRACKABLE_INDEX_VARIANCE_POSY] =
                0.05f * f32PrefactorNoiseY * f32dTpow5;
            /* PRQA S 3120 */ /* Mathematical formula */
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_VELX] =
                (f32PrefactorNoiseX * f32dTpow4) / FLT_EIGHT;
            matQ.data[TRACKABLE_INDEX_VARIANCE_VELX] =
                (f32PrefactorNoiseX * f32dTpow3) / FLT_THREE;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_VELY] =
                (f32PrefactorNoiseY * f32dTpow4) / FLT_EIGHT;
            matQ.data[TRACKABLE_INDEX_VARIANCE_VELY] =
                (f32PrefactorNoiseY * f32dTpow3) / FLT_THREE;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_ACCX] =
                (f32PrefactorNoiseX * f32dTpow3) / FLT_SIX;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_VELX_ACCX] =
                FLT_ONE_HALF * f32PrefactorNoiseX * f32dTpow2;
            matQ.data[TRACKABLE_INDEX_VARIANCE_ACCX] =
                f32PrefactorNoiseX * f32dT;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_ACCY] =
                (f32PrefactorNoiseY * f32dTpow3) / FLT_SIX;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_VELY_ACCY] =
                FLT_ONE_HALF * f32PrefactorNoiseY * f32dTpow2;
            matQ.data[TRACKABLE_INDEX_VARIANCE_ACCY] =
                f32PrefactorNoiseY * f32dT;
        } else if (TRACKABLE_ACCX < pTrkbl->vecX.nRows) {
            pTrkbl->vecX.data[TRACKABLE_POSX] =
                (pTrkbl->vecX.data[TRACKABLE_POSX] +
                 (f32dT * pTrkbl->vecX.data[TRACKABLE_VELX])) +
                (f32dTSquared * pTrkbl->vecX.data[TRACKABLE_ACCX]);
            pTrkbl->vecX.data[TRACKABLE_POSY] =
                pTrkbl->vecX.data[TRACKABLE_POSY] +
                (f32dT * pTrkbl->vecX.data[TRACKABLE_VELY]);
            pTrkbl->vecX.data[TRACKABLE_VELX] =
                pTrkbl->vecX.data[TRACKABLE_VELX] +
                (f32dT * pTrkbl->vecX.data[TRACKABLE_ACCX]);

            /* Init Q matrix for vx, vy, ax */
            /* PRQA S 3121 1 */ /* Mathematical formula */
            matQ.data[TRACKABLE_INDEX_VARIANCE_POSX] =
                0.05f * f32PrefactorNoiseX * f32dTpow5;
            /* PRQA S 3120 */ /* Mathematical formula */
            matQ.data[TRACKABLE_INDEX_VARIANCE_POSY] =
                (f32PrefactorNoiseY * f32dTpow3) / FLT_THREE;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_VELX] =
                (f32PrefactorNoiseX * f32dTpow4) / FLT_EIGHT;
            matQ.data[TRACKABLE_INDEX_VARIANCE_VELX] =
                (f32PrefactorNoiseX * f32dTpow3) / FLT_THREE;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_VELY] =
                FLT_ONE_HALF * f32PrefactorNoiseY * f32dTpow2;
            matQ.data[TRACKABLE_INDEX_VARIANCE_VELY] =
                f32PrefactorNoiseY * f32dT;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_ACCX] =
                (f32PrefactorNoiseX * f32dTpow3) / FLT_SIX;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_VELX_ACCX] =
                FLT_ONE_HALF * f32PrefactorNoiseX * f32dTpow2;
            matQ.data[TRACKABLE_INDEX_VARIANCE_ACCX] =
                f32PrefactorNoiseX * f32dT;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_ACCY] = FLT_ZERO;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_VELY_ACCY] = FLT_ZERO;
            matQ.data[TRACKABLE_INDEX_VARIANCE_ACCY] = FLT_ZERO;
        } else {
            pTrkbl->vecX.data[TRACKABLE_POSX] =
                pTrkbl->vecX.data[TRACKABLE_POSX] +
                (f32dT * pTrkbl->vecX.data[TRACKABLE_VELX]);
            pTrkbl->vecX.data[TRACKABLE_POSY] =
                pTrkbl->vecX.data[TRACKABLE_POSY] +
                (f32dT * pTrkbl->vecX.data[TRACKABLE_VELY]);

            /* Init Q Matrix for vx, vy */
            matQ.data[TRACKABLE_INDEX_VARIANCE_POSX] =
                (f32PrefactorNoiseX * f32dTpow3) / FLT_THREE;
            matQ.data[TRACKABLE_INDEX_VARIANCE_POSY] =
                (f32PrefactorNoiseY * f32dTpow3) / FLT_THREE;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_VELX] =
                FLT_ONE_HALF * f32PrefactorNoiseX * f32dTpow2;
            matQ.data[TRACKABLE_INDEX_VARIANCE_VELX] =
                f32PrefactorNoiseX * f32dT;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_VELY] =
                FLT_ONE_HALF * f32PrefactorNoiseY * f32dTpow2;
            matQ.data[TRACKABLE_INDEX_VARIANCE_VELY] =
                f32PrefactorNoiseY * f32dT;

            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_ACCX] = FLT_ZERO;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_VELX_ACCX] = FLT_ZERO;
            matQ.data[TRACKABLE_INDEX_VARIANCE_ACCX] = FLT_ZERO;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_ACCY] = FLT_ZERO;
            matQ.data[TRACKABLE_INDEX_COVARIANCE_VELY_ACCY] = FLT_ZERO;
            matQ.data[TRACKABLE_INDEX_VARIANCE_ACCY] = FLT_ZERO;
        }

        matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_POSY] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_VELX] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_VELY] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_VELX_VELY] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_POSY_ACCX] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_VELY_ACCX] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_POSX_ACCY] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_VELX_ACCY] = FLT_ZERO;
        matQ.data[TRACKABLE_INDEX_COVARIANCE_ACCX_ACCY] = FLT_ZERO;

        /** - Calculate new covariance matrix P according to LKF equations */
        /*   P = A * P * A' + Q; */
        u32Success = LKF_PredictSymMat(&pTrkbl->matP, f32dT);
        u32Success |= f32SymMatAddSymMat(&matQ, &pTrkbl->matP, &pTrkbl->matP);
    }

    return u32Success;
}

// ====================>
uint32 LKF_CoordinatedTurn_DoPredict(CONSTP2VAR(TueObjFusn_TrackableType,
                                                AUTOMATIC,
                                                ObjFusn_VAR_NOINIT) pTrkbl,
                                     const float32 f32PredictionDt) {
    uint32 u32Success = TUEOBJFUSN_ERROR_NOERROR;
    /* PRQA S 3204 1 */ // variable may be set to other values depending on
                        //    run-time check activation
    float32 f32Q_1_1;
    float32 f32Q_1_2;
    float32 f32Q_2_2;
    float32 f32HeadingYawRateCovar;
    float32 f32Tmp;

#if (TUE_PRV_RUNTIME_ERROR_CHECK == STD_ON)
#if (TUE_PRV_RUNTIME_POINTER_CHECK == STD_ON)
    if (NULL_PTR == pTrkbl) {
        u32Success = TUEOBJFUSN_ERROR_NULL_POINTER_EXCEPTION;
        (void)tue_prv_error_management_addError(
            u32Success, TUEOBJFUSN_AAU_COORDINATED_TURN,
            TUEOBJFUSN_AAU_LKF_COORDINATED_TURN_DO_PREDICT);
    } else
#endif
        if (pTrkbl->vecX.nRows > TUEOBJFUSN_MATRIX_SIZE) {
        u32Success = TUEOBJFUSN_ERROR_MATRIX_EXCEEDS_MAXIMUM_SIZE;
        (void)tue_prv_error_management_addError(
            u32Success, TUEOBJFUSN_AAU_COORDINATED_TURN,
            TUEOBJFUSN_AAU_LKF_COORDINATED_TURN_DO_PREDICT);
    } else if (pTrkbl->vecX.nRows != (pTrkbl->matP).u16Size) {
        u32Success = TUEOBJFUSN_ERROR_MATRIX_SIZE_NOT_EQUAL;
        (void)tue_prv_error_management_addError(
            u32Success, TUEOBJFUSN_AAU_COORDINATED_TURN,
            TUEOBJFUSN_AAU_LKF_COORDINATED_TURN_DO_PREDICT);
    } else if (pTrkbl->vecX.nRows <
               TUE_PRV_LKF_COORDINATED_TURN_MIN_STATE_SIZE) {
        u32Success = TUEOBJFUSN_ERROR_INPUT_NOT_ENOUGH_STATES;
        (void)tue_prv_error_management_addError(
            u32Success, TUEOBJFUSN_AAU_COORDINATED_TURN,
            TUEOBJFUSN_AAU_LKF_COORDINATED_TURN_DO_PREDICT);
    } else if (f32PredictionDt < FLT_ZERO) {
        u32Success = TUEOBJFUSN_ERROR_INVALID_INPUT;
        (void)tue_prv_error_management_addError(
            u32Success, TUEOBJFUSN_AAU_COORDINATED_TURN,
            TUEOBJFUSN_AAU_LKF_COORDINATED_TURN_DO_PREDICT);
    } else
#endif
    {
        f32Q_2_2 = TUE_PRV_COORDINATED_TURN_VARIANCE_IN_HEADING_FOR_Q *
                   f32PredictionDt;
        f32Q_1_2 = (f32PredictionDt * f32Q_2_2);
        f32Q_1_1 = FLT_ONE_THIRD * (f32PredictionDt * f32Q_1_2);
        f32Q_1_2 *= FLT_ONE_HALF;
        f32HeadingYawRateCovar = pTrkbl->f32CovarHeadingYawRate;

        f32Tmp = ((pTrkbl->vecX).data[TRACKABLE_VELX] *
                  (pTrkbl->vecX).data[TRACKABLE_VELX]) +
                 ((pTrkbl->vecX).data[TRACKABLE_VELY] *
                  (pTrkbl->vecX).data[TRACKABLE_VELY]);

        /* No prediction for stationary targets */
        if (f32Tmp > TUE_PRV_COORDINATED_TURN_MIN_TARGET_SPEED) {
            f32Tmp = f32PredictionDt * pTrkbl->f32YawRateVar;

            /* Predict using F-Matrix  (1 Delta T)    */
            /*                         (0    1   )    */
            pTrkbl->f32Heading += (f32PredictionDt * pTrkbl->f32YawRate);
            pTrkbl->f32Heading = tue_prv_fusion_norm_angle(
                pTrkbl->f32Heading);  // normalizied between [-pi pi]

            /* Predict variances and covariance */
            pTrkbl->f32HeadingVar +=
                f32PredictionDt *
                f32Tmp;  // add propagation of yaw rate variance
            pTrkbl->f32HeadingVar +=
                (FLT_TWO *
                 (f32PredictionDt *
                  f32HeadingYawRateCovar));     // add propagation of covariance
            pTrkbl->f32HeadingVar += f32Q_1_1;  // add process noise
            
            pTrkbl->f32CovarHeadingYawRate +=
                f32Tmp;  // add propagation of yaw rate variance
            pTrkbl->f32CovarHeadingYawRate += f32Q_1_2;  // add process noise
            pTrkbl->f32YawRateVar += f32Q_2_2;           // add process noise
        } else {
            pTrkbl->f32YawRate = FLT_ZERO;
        }
        pTrkbl->f32HeadingVar = tue_prv_fusion_min_max_F32(
            pTrkbl->f32HeadingVar, TUEOBJFUSN_TRACKABLE_F32HEADINGVAR_MIN,
            TUEOBJFUSN_TRACKABLE_F32HEADINGVAR_MAX);
        pTrkbl->f32YawRateVar = tue_prv_fusion_min_max_F32(
            pTrkbl->f32YawRateVar, TUEOBJFUSN_TRACKABLE_F32YAWRATEVAR_MIN,
            TUEOBJFUSN_TRACKABLE_F32YAWRATEVAR_MAX);
    }

    return u32Success;
}