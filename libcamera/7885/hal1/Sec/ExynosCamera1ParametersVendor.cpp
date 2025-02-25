/*
**
** Copyright 2013, Samsung Electronics Co. LTD
**
** Licensed under the Apache License, Version 2.0 (the "License");
** you may not use this file except in compliance with the License.
** You may obtain a copy of the License at
**
**     http://www.apache.org/licenses/LICENSE-2.0
**
** Unless required by applicable law or agreed to in writing, software
** distributed under the License is distributed on an "AS IS" BASIS,
** WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
** See the License for the specific language governing permissions and
** limitations under the License.
*/

/* #define LOG_NDEBUG 0 */
#define LOG_TAG "ExynosCamera1Parameters"
#include <cutils/log.h>

#include "ExynosCamera1Parameters.h"
#include "SecCameraUtil.h"

namespace android {

void ExynosCamera1Parameters::vendorSpecificConstructor(int cameraId, bool flagCompanion)
{
    char propertyValue[PROPERTY_VALUE_MAX];

#ifdef SAMSUNG_DNG
    m_setDngFixedAttribute();
#endif

    mDebugInfo.num_of_appmarker = 1; /* Default : APP4 */
    mDebugInfo.idx[0][0] = APP_MARKER_4; /* matching the app marker 4 */

    /* DebugInfo2 */
    mDebugInfo2.num_of_appmarker = 1; /* Default : APP4 */
    mDebugInfo2.idx[0][0] = APP_MARKER_4; /* matching the app marker 4 */

#ifdef SAMSUNG_OIS
    if (cameraId == CAMERA_ID_BACK) {
        mDebugInfo.debugSize[APP_MARKER_4] = sizeof(struct camera2_udm) + sizeof(struct ois_exif_data);
    } else {
        mDebugInfo.debugSize[APP_MARKER_4] = sizeof(struct camera2_udm);
    }
#else
    mDebugInfo.debugSize[APP_MARKER_4] = sizeof(struct camera2_udm);
#endif

#ifdef SAMSUNG_MTF
    mDebugInfo.debugSize[APP_MARKER_4] += sizeof(struct mtf_exif_data);
#endif

    // Check debug_attribute_t struct in ExynosExif.h
#ifdef SAMSUNG_LLS_DEBLUR
    mDebugInfo.debugSize[APP_MARKER_4] += sizeof(struct lls_exif_data);
#endif
#ifdef SAMSUNG_LENS_DC
    mDebugInfo.debugSize[APP_MARKER_4] += sizeof(struct ldc_exif_data);
#endif
#ifdef SAMSUNG_STR_CAPTURE
    mDebugInfo.debugSize[APP_MARKER_4] += sizeof(struct str_exif_data);
#endif
    mDebugInfo.debugSize[APP_MARKER_4] += sizeof(struct sensor_id_exif_data);

    mDebugInfo.debugData[APP_MARKER_4] = new char[mDebugInfo.debugSize[APP_MARKER_4]];
    memset((void *)mDebugInfo.debugData[APP_MARKER_4], 0, mDebugInfo.debugSize[APP_MARKER_4]);

    /* DebugInfo2 */
    mDebugInfo2.debugSize[APP_MARKER_4] = mDebugInfo.debugSize[APP_MARKER_4];
    mDebugInfo2.debugData[APP_MARKER_4] = new char[mDebugInfo2.debugSize[APP_MARKER_4]];
    memset((void *)mDebugInfo2.debugData[APP_MARKER_4], 0, mDebugInfo2.debugSize[APP_MARKER_4]);

    // Check debug_attribute_t struct in ExynosExif.h
    mDebugInfo.debugSize[APP_MARKER_5] = 0;
#ifdef SAMSUNG_BD
    mDebugInfo.idx[1][0] = APP_MARKER_5;
    mDebugInfo.debugSize[APP_MARKER_5] += sizeof(struct bd_exif_data);
#endif
#ifdef SAMSUNG_UTC_TS
    mDebugInfo.idx[1][0] = APP_MARKER_5; /* mathcing the app marker 5 */
    mDebugInfo.debugSize[APP_MARKER_5] += sizeof(struct utc_ts);
#endif

    if (mDebugInfo.idx[1][0] == APP_MARKER_5 && mDebugInfo.debugSize[APP_MARKER_5] != 0) {
        mDebugInfo.num_of_appmarker++;
        mDebugInfo.debugData[APP_MARKER_5] = new char[mDebugInfo.debugSize[APP_MARKER_5]];
        memset((void *)mDebugInfo.debugData[APP_MARKER_5], 0, mDebugInfo.debugSize[APP_MARKER_5]);

        /* DebugInfo2 */
        mDebugInfo2.idx[1][0] = APP_MARKER_5;
        mDebugInfo2.num_of_appmarker++;
        mDebugInfo2.debugSize[APP_MARKER_5] = mDebugInfo.debugSize[APP_MARKER_5];
        mDebugInfo2.debugData[APP_MARKER_5] = new char[mDebugInfo2.debugSize[APP_MARKER_5]];
        memset((void *)mDebugInfo2.debugData[APP_MARKER_5], 0, mDebugInfo2.debugSize[APP_MARKER_5]);
    }

    // CAUTION!! : Initial values must be prior to setDefaultParameter() function.
    // Initial Values : START
#if defined(SAMSUNG_COMPANION) || defined(SAMSUNG_EEPROM)
    m_romReadThreadDone = false;
    m_use_companion = flagCompanion;
#endif
    CLOGV("flagCompanion(%d)", flagCompanion);
#ifdef LLS_CAPTURE
    m_flagLLSOn = false;
    m_LLSCaptureOn = false;
    m_LLSValue = 0;
#endif
#ifdef SR_CAPTURE
    m_flagSRSOn = false;
#endif
#if defined(SR_CAPTURE) || defined(SAMSUNG_HIFI_LLS)
    m_flagNoBayerCrop = false;
#endif
#ifdef OIS_CAPTURE
    m_flagOISCaptureOn = false;
#endif
#ifdef SAMSUNG_DNG
    m_flagDNGCaptureOn = false;
    m_use_DNGCapture = false;
    m_flagMultiFrame = false;
    m_isUsefulDngInfo = false;
    m_dngSaveLocation = BURST_SAVE_PHONE;
#endif
#ifdef SAMSUNG_HRM
    m_flagSensorHRM_Hint = false;
#endif
#ifdef SAMSUNG_LIGHT_IR
    m_flagSensorLight_IR_Hint = false;
#endif
#ifdef SAMSUNG_GYRO
    m_flagSensorGyroHint = false;
    /* Initialize gyro value for inital-calibration */
    m_metadata.shot.uctl.aaUd.gyroInfo.x = 1234;
    m_metadata.shot.uctl.aaUd.gyroInfo.y = 1234;
    m_metadata.shot.uctl.aaUd.gyroInfo.z = 1234;
#endif
#ifdef SAMSUNG_ACCELEROMETER
    m_flagSensorAccelerationHint = false;
    /* Initialize accelerometer value for inital-calibration */
    m_metadata.shot.uctl.aaUd.accInfo.x = 1234;
    m_metadata.shot.uctl.aaUd.accInfo.y = 1234;
    m_metadata.shot.uctl.aaUd.accInfo.z = 1234;
#endif
#if defined(SAMSUNG_COMPANION) || defined(SAMSUNG_EEPROM)
    m_useFastenAeStable = isFastenAeStable(cameraId, m_use_companion);
#endif
#ifdef SAMSUNG_LLV
    m_isLLVOn = true;
#endif
#ifdef SAMSUNG_OT
    m_startObjectTracking = false;
    m_objectTrackingAreaChanged = false;
    m_objectTrackingGet = false;
    int maxNumFocusAreas = getMaxNumFocusAreas();
    m_objectTrackingArea = new ExynosRect2[maxNumFocusAreas];
    m_objectTrackingWeight = new int[maxNumFocusAreas];
#endif
#ifdef SAMSUNG_LBP
    m_useBestPic = USE_LIVE_BEST_PIC;
#endif
#ifdef SAMSUNG_JQ
    m_isJpegQtableOn = false;
    m_jpegQtableStatus = JPEG_QTABLE_DEINIT;
#endif
#ifdef SAMSUNG_DOF
    m_curLensStep = 0;
    m_curLensCount = 0;
#endif
#ifdef USE_BINNING_MODE
    m_binningProperty = checkBinningProperty();
#endif
#ifdef USE_LIMITATION_FOR_THIRD_PARTY
    m_fpsProperty = checkFpsProperty();
#endif
#ifdef SAMSUNG_OIS
    m_oisNode = NULL;
    m_setOISmodeSetting = false;
#ifdef OIS_CAPTURE
    m_llsValue = 0;
#endif
#endif
#ifdef FORCE_CAL_RELOAD
    m_calValid = true;
#endif
#ifdef USE_FADE_IN_ENTRANCE
    m_flagFirstEntrance = false;
#endif
#ifdef SAMSUNG_LLS_DEBLUR
    m_flagLDCaptureMode = 0;
    m_LDCaptureCount = 0;
#endif
#ifdef SUPPORT_DEPTH_MAP
    m_flaguseDepthMap = false;
    m_flagDepthCallbackOnPreview = false;
    m_flagDepthCallbackOnCapture = false;
#endif
#ifdef FLASHED_LLS_CAPTURE
    m_isFlashedLLSCapture = false;
#endif
#ifdef USE_MULTI_FACING_SINGLE_CAMERA
    m_cameraDirection = 1;
#endif
#if defined(BURST_CAPTURE) && defined(VARIABLE_BURST_FPS)
    m_burstShotDuration = NORMAL_BURST_DURATION;
#endif
#ifdef SAMSUNG_QUICK_SWITCH
    m_isQuickSwitch = false;
    m_quickSwitchCmd = QUICK_SWITCH_CMD_DONE;
#endif
#ifdef SAMSUNG_COMPANION
    m_flagChangedRtHdr = false;
#endif
#ifdef SAMSUNG_LENS_DC
    m_flagLensDCEnable = false;
    if (cameraId == CAMERA_ID_BACK) {
        m_flagLensDCMode = true;
    } else {
        m_flagLensDCMode = false;
    }
#endif
#ifdef SAMSUNG_STR_CAPTURE
    m_flagSTRCaptureEnable = false;
#endif
#ifdef SAMSUNG_FOCUS_PEAKING
    m_flagFocusPeakingMode = false;
#endif
    m_flagMotionPhotoOn = false;

#ifdef USE_ODC_CAPTURE
    m_flagODCCaptureMode = false;
    m_flagODCCaptureEnable = false;
#endif

#ifdef SAMSUNG_DUAL_SOLUTION
    m_needPreviewMargin = 0;
    m_fusionParam = NULL;
#endif

#ifdef USE_REMOSAIC_CAPTURE
    m_flagRemosaicCaptureMode = REMOSAIC_CAPTURE_MODE_NONE;
#endif

#ifdef USE_DUAL_CAMERA
    m_dualParameters->registerCameraParam(cameraId, &m_params);
#endif

    if (m_scenario == SCENARIO_SECURE) {
        /* set initial value for Secure Camera*/
        m_shutterSpeed = 332;// 33.2ms
        m_gain = 20;         // 2.0
        m_irLedWidth = 240;  // 24ms
        m_irLedDelay = 0;    // 54ms
        m_irLedCurrent = 5;  // 450mA

        property_get("ro.factory.factory_binary", propertyValue, "0");
        if (!strncmp(propertyValue, "factory", 7)) {
            m_irLedOnTime = 0;  // 0s, for factory binary
        } else {
            m_irLedOnTime = 10;  // 10s
        }
    }

#ifdef SAMSUNG_OIS_VDIS
    setSWVdisOISInit();
#endif
}

void ExynosCamera1Parameters::operator=(const ExynosCamera1Parameters& obj)
{
    (*this).m_params.unflatten(obj.m_params.flatten());
    memcpy(&(*this).m_metadata, &obj.m_metadata, sizeof(struct camera2_shot_ext));

    memcpy(&(*this).m_cameraInfo, &obj.m_cameraInfo, sizeof(struct exynos_camera_info));

    (*this).m_enabledMsgType =  obj.m_enabledMsgType;

    (*this).m_calculatedHorizontalViewAngle = obj.m_calculatedHorizontalViewAngle;

    (*this).m_previewRunning = obj.m_previewRunning;
    (*this).m_previewSizeChanged = obj.m_previewSizeChanged;
    (*this).m_pictureRunning = obj.m_pictureRunning;
    (*this).m_recordingRunning = obj.m_recordingRunning;
    (*this).m_flagCheckDualMode = obj.m_flagCheckDualMode;
#ifdef USE_DUAL_CAMERA
    (*this).m_flagCheckDualCameraMode = obj.m_flagCheckDualCameraMode;
    (*this).m_flagDualCameraPreviewFusionMode = obj.m_flagDualCameraPreviewFusionMode;
    (*this).m_flagDualCameraCaptureFusionMode = obj.m_flagDualCameraCaptureFusionMode;
#endif

#if defined(SAMSUNG_COMPANION) || defined(SAMSUNG_EEPROM)
    (*this).m_romReadThreadDone = obj.m_romReadThreadDone;
    (*this).m_use_companion = obj.m_use_companion;
#endif

    (*this).m_IsThumbnailCallbackOn = obj.m_IsThumbnailCallbackOn;
    (*this).m_flagRestartPreviewChecked = obj.m_flagRestartPreviewChecked;
    (*this).m_flagRestartPreview = obj.m_flagRestartPreview;
    (*this).m_fastFpsMode = obj.m_fastFpsMode;
    (*this).m_flagFirstStart = obj.m_flagFirstStart;
    (*this).m_flagMeteringRegionChanged = obj.m_flagMeteringRegionChanged;
    (*this).m_flagHWVDisMode = obj.m_flagHWVDisMode;

    (*this).m_flagVideoStabilization = obj.m_flagVideoStabilization;
    (*this).m_flag3dnrMode = obj.m_flag3dnrMode;

#ifdef  LLS_CAPTURE
    (*this).m_flagLLSOn = obj.m_flagLLSOn;
    (*this).m_LLSValue = obj.m_LLSValue;
    (*this).m_LLSCaptureOn = obj.m_LLSCaptureOn;
#ifdef  LLS_REPROCESSING
    (*this).m_LLSCaptureCount = obj.m_LLSCaptureCount;
#endif
#endif

#ifdef  SR_CAPTURE
    (*this).m_flagSRSOn = obj.m_flagSRSOn;
#endif

#if defined(SR_CAPTURE) || defined(SAMSUNG_HIFI_LLS)
    (*this).m_flagNoBayerCrop = obj.m_flagNoBayerCrop;
#endif

#ifdef  OIS_CAPTURE
    (*this).m_flagOISCaptureOn = obj.m_flagOISCaptureOn;
    (*this).m_llsValue = obj.m_llsValue;
#endif

#ifdef  SAMSUNG_DNG
    (*this).m_flagDNGCaptureOn = obj.m_flagDNGCaptureOn;
    (*this).m_use_DNGCapture = obj.m_use_DNGCapture;
    (*this).m_flagMultiFrame = obj.m_flagMultiFrame;
    (*this).m_isUsefulDngInfo = obj.m_isUsefulDngInfo;
    (*this).m_dngInfo = obj.m_dngInfo;
#endif

#ifdef  RAWDUMP_CAPTURE
    (*this).m_flagRawCaptureOn = obj.m_flagRawCaptureOn;
#endif

#ifdef  SAMSUNG_LLS_DEBLUR
    (*this).m_flagLDCaptureMode = obj.m_flagLDCaptureMode;
    (*this).m_flagLDCaptureLLSValue = obj.m_flagLDCaptureLLSValue;
    (*this).m_LDCaptureCount = obj.m_LDCaptureCount;
#endif

    (*this).m_flagCheckRecordingHint = obj.m_flagCheckRecordingHint;

#ifdef  SAMSUNG_HRM
    (*this).m_flagSensorHRM_Hint = obj.m_flagSensorHRM_Hint;
#endif

#ifdef  SAMSUNG_LIGHT_IR
    (*this).m_flagSensorLight_IR_Hint = obj.m_flagSensorLight_IR_Hint;
#endif

#ifdef  SAMSUNG_GYRO
    (*this).m_flagSensorGyroHint = obj.m_flagSensorGyroHint;
#endif

#ifdef SAMSUNG_ACCELEROMETER
    (*this).m_flagSensorAccelerationHint = obj.m_flagSensorAccelerationHint;
#endif

    (*this).m_setfile = obj.m_setfile;
    (*this).m_yuvRange = obj.m_yuvRange;
    (*this).m_setfileReprocessing = obj.m_setfileReprocessing;
    (*this).m_yuvRangeReprocessing = obj.m_yuvRangeReprocessing;

#ifdef  USE_BINNING_MODE
    (*this).m_binningProperty = obj.m_binningProperty;
#endif

    (*this).m_useSizeTable = obj.m_useSizeTable;
    (*this).m_useDynamicBayer = obj.m_useDynamicBayer;
    (*this).m_useDynamicBayerVideoSnapShot = obj.m_useDynamicBayerVideoSnapShot;
    (*this).m_useDynamicScc = obj.m_useDynamicScc;
    (*this).m_useFastenAeStable = obj.m_useFastenAeStable;
    (*this).m_usePureBayerReprocessing = obj.m_usePureBayerReprocessing;

#ifdef  SAMSUNG_LBP
    (*this).m_useBestPic = obj.m_useBestPic;
#endif

    (*this).m_useAdaptiveCSCRecording = obj.m_useAdaptiveCSCRecording;
    (*this).m_dvfsLock = obj.m_dvfsLock;
    (*this).m_previewBufferCount = obj.m_previewBufferCount;

    (*this).m_reallocBuffer = obj.m_reallocBuffer;

#ifdef  USE_FADE_IN_ENTRANCE
    (*this).m_flagFirstEntrance = obj.m_flagFirstEntrance;
#endif

    (*this).m_exynosconfig->mode = obj.m_exynosconfig->mode;
    (*this).m_exynosconfig->current = obj.m_exynosconfig->current;

#ifdef  SAMSUNG_LLV
    (*this).m_isLLVOn = obj.m_isLLVOn;
#endif

#ifdef  SAMSUNG_DOF
    (*this).m_curLensStep = obj.m_curLensStep;
    (*this).m_curLensCount = obj.m_curLensCount;
#endif

    (*this).m_setFocusmodeSetting = obj.m_setFocusmodeSetting;

#ifdef  USE_CSC_FEATURE
    memcpy((*this).m_antiBanding, &obj.m_antiBanding, sizeof(char) * 10);
#endif

#ifdef  SAMSUNG_OIS
    (*this).m_oisNode = obj.m_oisNode;
    (*this).m_setOISmodeSetting = obj.m_setOISmodeSetting;
#endif

    (*this).m_zoom_activated = obj.m_zoom_activated;
    (*this).m_firing_flash_marking = obj.m_firing_flash_marking;
    (*this).m_halVersion = obj.m_halVersion;

#ifdef  SAMSUNG_OT
    (*this).m_startObjectTracking = obj.m_startObjectTracking;
    (*this).m_objectTrackingAreaChanged = obj.m_objectTrackingAreaChanged;
    (*this).m_objectTrackingGet = obj.m_objectTrackingGet;
    memcpy((*this).m_objectTrackingArea, &obj.m_objectTrackingArea, sizeof(ExynosRect2) * getMaxNumFocusAreas());
    memcpy((*this).m_objectTrackingWeight, &obj.m_objectTrackingWeight, sizeof(int) * getMaxNumFocusAreas());
#endif

#ifdef  SAMSUNG_LBP
    (*this).m_normal_best_frame_count = obj.m_normal_best_frame_count;
    (*this).m_scp_frame_count = obj.m_scp_frame_count;
    (*this).m_bayer_frame_count = obj.m_bayer_frame_count;
#endif

#ifdef  SAMSUNG_JQ
    memcpy((*this).m_jpegQtable, &obj.m_jpegQtable, sizeof(unsigned char) * 128);
    (*this).m_jpegQtableStatus = obj.m_jpegQtableStatus;
    (*this).m_isJpegQtableOn = obj.m_isJpegQtableOn;
#endif

#ifdef  FORCE_CAL_RELOAD
    (*this).m_calValid = obj.m_calValid;
#endif

#ifdef SAMSUNG_QUICK_SWITCH
    (*this).m_isQuickSwitch = obj.m_isQuickSwitch;
    (*this).m_quickSwitchCmd = obj.m_quickSwitchCmd;
#endif

#ifdef SAMSUNG_DUAL_SOLUTION
    (*this).m_needPreviewMargin = obj.m_needPreviewMargin;
#endif

    (*this).m_exposureTimeCapture = obj.m_exposureTimeCapture;

    (*this).m_zoomWithScaler = obj.m_zoomWithScaler;

    (*this).m_flagMotionPhotoOn = obj.m_flagMotionPhotoOn;
}

status_t ExynosCamera1Parameters::setRestartParameters(const CameraParameters& params)
{
    status_t ret = NO_ERROR;

#ifdef USE_FADE_IN_ENTRANCE
    if (checkFirstEntrance(params) != NO_ERROR) {
        CLOGE(" checkFirstEntrance faild");
    }
#endif
    /* Return OK means that the vision mode is enabled */
    if (checkVisionMode(params) != NO_ERROR) {
        CLOGE(" checkVisionMode fail");
        return BAD_VALUE;
    }

    if (getVisionMode() == true) {
        CLOGD(" Vision mode enabled");
        return NO_ERROR;
    }

    if (checkRecordingHint(params) != NO_ERROR)
        CLOGE(" checkRecordingHint fail");

#ifdef USE_DUAL_CAMERA
    if (checkDualCameraMode(params) != NO_ERROR)
        CLOGE(" checkDualCameraMode fail");
#endif

    if (checkDualMode(params) != NO_ERROR)
        CLOGE(" checkDualMode fail");

    if (checkDualRecordingHint(params) != NO_ERROR)
        CLOGE(" checkDualRecordingHint fail");

    if (checkEffectHint(params) != NO_ERROR)
        CLOGE(" checkEffectHint fail");

    if (checkEffectRecordingHint(params) != NO_ERROR)
        CLOGE(" checkEffectRecordingHint fail");

    if (checkPreviewFps(params) != NO_ERROR) {
        CLOGE(" checkPreviewFps fail");
        return BAD_VALUE;
    }

    if (getRecordingRunning() == false) {
        if (checkVideoSize(params) != NO_ERROR)
            CLOGE(" checkVideoSize fail");
    }

    if (getCameraId() == CAMERA_ID_BACK) {
        if (checkFastFpsMode(params) != NO_ERROR)
            CLOGE(" checkFastFpsMode fail");
    }

    if (checkPreviewSize(params) != NO_ERROR) {
        CLOGE(" checkPreviewSize fail");
        return BAD_VALUE;
    }

    if (checkPreviewFormat(params) != NO_ERROR)
        CLOGE(" checkPreviewFormat fail");

#ifdef SAMSUNG_COMPANION
    if (checkSceneMode(params) != NO_ERROR) {
        CLOGE(" checkSceneMode fail");
        return BAD_VALUE;
    }

    if (getSceneMode() != SCENE_MODE_HDR) {
        if (checkRTHdr(params) != NO_ERROR)
            CLOGE(" checkRTHdr fail");
    }
#endif

    if (m_getRestartPreviewChecked() == true) {
        CLOGD("Need restart preview");
        m_setRestartPreview(m_flagRestartPreviewChecked);
    }

    return ret;

}

status_t ExynosCamera1Parameters::setParameters(const CameraParameters& params)
{
    status_t ret = NO_ERROR;

#ifdef TEST_GED_HIGH_SPEED_RECORDING
    int minFpsRange = 0, maxFpsRange = 0;
    int frameRate = 0;

    params.getPreviewFpsRange(&minFpsRange, &maxFpsRange);
    frameRate = params.getPreviewFrameRate();
    CLOGD("getFastFpsMode=%d, maxFpsRange=%d, frameRate=%d",
        getFastFpsMode(), maxFpsRange, frameRate);
    if (frameRate == 60) {
        setFastFpsMode(1);
    } else if (frameRate == 120) {
        setFastFpsMode(2);
    } else if (frameRate == 240) {
        setFastFpsMode(3);
    } else {
        setFastFpsMode(0);
    }

    CLOGD("getFastFpsMode=%d", getFastFpsMode());
#endif
#ifdef USE_FADE_IN_ENTRANCE
    if (checkFirstEntrance(params) != NO_ERROR) {
        CLOGE("checkFirstEntrance faild");
    }
#endif
    /* Return OK means that the vision mode is enabled */
    if (checkVisionMode(params) != NO_ERROR) {
        CLOGE(" checkVisionMode fail");
        return BAD_VALUE;
    }

    if (getVisionMode() == true) {
        CLOGD(" Vision mode enabled");
    }

#ifdef USE_FW_OPMODE
    if (checkFactorytest(params) != NO_ERROR) {
        CLOGE(" checkFactorytest fail");
    }
#endif

    if (checkRecordingHint(params) != NO_ERROR)
        CLOGE(" checkRecordingHint fail");

#ifdef USE_DUAL_CAMERA
    if (checkDualCameraMode(params) != NO_ERROR)
        CLOGE(" checkDualCameraMode fail");
#endif

    if (checkDualMode(params) != NO_ERROR)
        CLOGE(" checkDualMode fail");

    if (checkDualRecordingHint(params) != NO_ERROR)
        CLOGE(" checkDualRecordingHint fail");

    if (checkEffectHint(params) != NO_ERROR)
        CLOGE(" checkEffectHint fail");

    if (checkEffectRecordingHint(params) != NO_ERROR)
        CLOGE(" checkEffectRecordingHint fail");

    if (checkPreviewFps(params) != NO_ERROR) {
        CLOGE(" checkPreviewFps fail");
        return BAD_VALUE;
    }

    if (getRecordingRunning() == false) {
        if (checkVideoSize(params) != NO_ERROR)
            CLOGE(" checkVideoSize fail");
    }

    if (getCameraId() == CAMERA_ID_BACK) {
        if (checkFastFpsMode(params) != NO_ERROR)
            CLOGE(" checkFastFpsMode fail");
    }

    if (checkVideoStabilization(params) != NO_ERROR)
        CLOGE(" checkVideoStabilization fail");

    if (checkSWVdisMode(params) != NO_ERROR)
        CLOGE(" checkSWVdisMode fail");

    bool swVdisUIMode = false;
#ifdef SUPPORT_SW_VDIS
    swVdisUIMode = getVideoStabilization();
#endif /*SUPPORT_SW_VDIS*/
    m_setSWVdisUIMode(swVdisUIMode);

    if (checkVtMode(params) != NO_ERROR)
        CLOGE(" checkVtMode fail");

    if (checkPreviewSize(params) != NO_ERROR) {
        CLOGE(" checkPreviewSize fail");
        return BAD_VALUE;
    }
    if (checkHWVdisMode(params) != NO_ERROR)
        CLOGE(" checkHWVdisMode fail");

    if (check3dnrMode(params) != NO_ERROR)
        CLOGE(" check3dnrMode fail");

    if (checkDrcMode(params) != NO_ERROR)
        CLOGE(" checkDrcMode fail");

    if (checkOdcMode(params) != NO_ERROR)
        CLOGE(" checkOdcMode fail");

    if (checkPreviewFormat(params) != NO_ERROR)
        CLOGE(" checkPreviewFormat fail");

    if (checkPictureSize(params) != NO_ERROR) {
        CLOGE(" checkPictureSize fail");
        return BAD_VALUE;
    }

    if (checkPictureFormat(params) != NO_ERROR) {
        CLOGE(" checkPictureFormat fail");
        return BAD_VALUE;
    }

    if (checkJpegQuality(params) != NO_ERROR)
        CLOGE(" checkJpegQuality fail");

    if (checkThumbnailSize(params) != NO_ERROR)
        CLOGE(" checkThumbnailSize fail");

    if (checkThumbnailQuality(params) != NO_ERROR)
        CLOGE(" checkThumbnailQuality fail");

    if (checkZoomLevel(params) != NO_ERROR) {
        CLOGE(" checkZoomLevel fail");
        return BAD_VALUE;
    }

    if (checkRotation(params) != NO_ERROR)
        CLOGE(" checkRotation fail");

    if (checkAutoExposureLock(params) != NO_ERROR)
        CLOGE(" checkAutoExposureLock fail");

    if (checkExposureCompensation(params) != NO_ERROR) {
        CLOGE(" checkExposureCompensation fail");
        return BAD_VALUE;
    }

    if (checkExposureTime(params) != NO_ERROR) {
        CLOGE(" checkExposureTime fail");
        return BAD_VALUE;
    }

    if (checkMeteringMode(params) != NO_ERROR)
        CLOGE(" checkMeteringMode fail");

    if (checkMeteringAreas(params) != NO_ERROR) {
        CLOGE(" checkMeteringAreas fail");
        return BAD_VALUE;
    }

    if (checkAntibanding(params) != NO_ERROR)
        CLOGE(" checkAntibanding fail");

    if (checkSceneMode(params) != NO_ERROR) {
        CLOGE(" checkSceneMode fail");
        return BAD_VALUE;
    }

    if (checkFocusMode(params) != NO_ERROR) {
        CLOGE(" checkFocusMode fail");
        return BAD_VALUE;
    }

    if (checkFlashMode(params) != NO_ERROR) {
        CLOGE(" checkFlashMode fail");
        return BAD_VALUE;
    }

    if (checkWhiteBalanceMode(params) != NO_ERROR) {
        CLOGE(" checkWhiteBalanceMode fail");
        return BAD_VALUE;
    }

    if (checkAutoWhiteBalanceLock(params) != NO_ERROR)
        CLOGE(" checkAutoWhiteBalanceLock fail");

#ifdef SAMSUNG_FOOD_MODE
    if (checkWbLevel(params) != NO_ERROR) {
        CLOGE(" checkWbLevel fail");
        return BAD_VALUE;
    }
#endif

    if (checkWbKelvin(params) != NO_ERROR) {
        CLOGE(" checkWbKelvin fail");
        return BAD_VALUE;
    }

    if (checkFocusAreas(params) != NO_ERROR) {
        CLOGE(" checkFocusAreas fail");
        return BAD_VALUE;
    }

#ifdef SAMSUNG_MANUAL_FOCUS
    if (checkFocusDistance(params) != NO_ERROR) {
        CLOGE(" checkFocusDistance fail");
        return BAD_VALUE;
    }
#endif

    if (checkColorEffectMode(params) != NO_ERROR) {
        CLOGE(" checkColorEffectMode fail");
        return BAD_VALUE;
    }

    if (checkGpsAltitude(params) != NO_ERROR)
        CLOGE(" checkGpsAltitude fail");

    if (checkGpsLatitude(params) != NO_ERROR)
        CLOGE(" checkGpsLatitude fail");

    if (checkGpsLongitude(params) != NO_ERROR)
        CLOGE(" checkGpsLongitude fail");

    if (checkGpsProcessingMethod(params) != NO_ERROR)
        CLOGE(" checkGpsProcessingMethod fail");

    if (checkGpsTimeStamp(params) != NO_ERROR)
        CLOGE(" checkGpsTimeStamp fail");

#if 0
    if (checkCityId(params) != NO_ERROR)
        CLOGE(" checkCityId fail");

    if (checkWeatherId(params) != NO_ERROR)
        CLOGE(" checkWeatherId fail");
#endif

    if (checkBrightness(params) != NO_ERROR)
        CLOGE(" checkBrightness fail");

    if (checkSaturation(params) != NO_ERROR)
        CLOGE(" checkSaturation fail");

    if (checkSharpness(params) != NO_ERROR)
        CLOGE(" checkSharpness fail");

    if (checkHue(params) != NO_ERROR)
        CLOGE(" checkHue fail");

#ifdef SAMSUNG_COMPANION
    if (getSceneMode() != SCENE_MODE_HDR) {
        if (checkRTDrc(params) != NO_ERROR)
            CLOGE(" checkRTDrc fail");
    }

    if (checkPaf(params) != NO_ERROR)
        CLOGE(" checkPaf fail");

    if (getSceneMode() != SCENE_MODE_HDR) {
        if (checkRTHdr(params) != NO_ERROR)
            CLOGE(" checkRTHdr fail");
    }
#endif

    if (checkIso(params) != NO_ERROR)
        CLOGE(" checkIso fail");

    if (checkContrast(params) != NO_ERROR)
        CLOGE(" checkContrast fail");

    if (checkHdrMode(params) != NO_ERROR)
        CLOGE(" checkHdrMode fail");

    if (checkWdrMode(params) != NO_ERROR)
        CLOGE(" checkWdrMode fail");

    if (checkShotMode(params) != NO_ERROR)
        CLOGE(" checkShotMode fail");

    if (checkAntiShake(params) != NO_ERROR)
        CLOGE(" checkAntiShake fail");

    if (checkGamma(params) != NO_ERROR)
        CLOGE(" checkGamma fail");

    if (checkVRMode(params) != NO_ERROR)
        CLOGE(" checkVRMode fail");

    if (checkSlowAe(params) != NO_ERROR)
        CLOGE(" checkSlowAe fail");

    if (checkScalableSensorMode(params) != NO_ERROR)
        CLOGE(" checkScalableSensorMode fail");

    if (checkImageUniqueId(params) != NO_ERROR)
        CLOGE(" checkImageUniqueId fail");

    if (checkSeriesShotMode(params) != NO_ERROR)
        CLOGE(" checkSeriesShotMode fail");

#ifdef BURST_CAPTURE
    if (checkSeriesShotFilePath(params) != NO_ERROR)
        CLOGE(" checkSeriesShotFilePath fail");
#endif

#ifdef SAMSUNG_DNG
    if (checkDngFilePath(params) != NO_ERROR)
        CLOGE(" checkDngFilePath fail");
#endif

#ifdef SAMSUNG_LLV
    if (checkLLV(params) != NO_ERROR)
        CLOGE(" checkLLV fail");
#endif

#ifdef SAMSUNG_DOF
    if (checkMoveLens(params) != NO_ERROR)
        CLOGE(" checkMoveLens fail");
#endif

#ifdef SAMSUNG_OIS
    if (checkOIS(params) != NO_ERROR)
        CLOGE(" checkOIS fail");
#endif

    if (m_getRestartPreviewChecked() == true) {
        CLOGD("Need restart preview");
        m_setRestartPreview(m_flagRestartPreviewChecked);
    }

    if (checkSetfileYuvRange() != NO_ERROR)
        CLOGE(" checkSetfileYuvRange fail");

#ifdef SAMSUNG_QUICKSHOT
    if (checkQuickShot(params) != NO_ERROR)
        CLOGE(" checkQuichShot fail");
#endif
#ifdef SAMSUNG_HYPER_MOTION
    if (checkHyperMotionSpeed(params) != NO_ERROR)
        CLOGE(" checkHyperMotionSpeed fail");
#endif
#ifdef USE_MULTI_FACING_SINGLE_CAMERA
    if (checkCameraDirection(params) != NO_ERROR)
        CLOGE(" checkCameraDirection fail");
#endif
#ifdef SUPPORT_MULTI_AF
    if (checkMultiAf(params) != NO_ERROR)
        CLOGE(" checkMultiAf fail");
#endif
#ifdef SAMSUNG_LENS_DC
    if (checkLensDCMode(params) != NO_ERROR)
        CLOGE(" checkLensDCMode fail");
#endif
    checkHorizontalViewAngle();

#ifdef SUPPORT_DEPTH_MAP
    checkUseDepthMap();
#endif
    if (m_scenario == SCENARIO_SECURE) {
        if (checkShutterSpeed(params) != NO_ERROR)
            CLOGE(" checkShutterSpeed fail");

        if (checkGain(params) != NO_ERROR)
            CLOGE(" checkGain fail");

        if (checkIrLedWidth(params) != NO_ERROR)
            CLOGE(" checkIrLedWidth fail");

        if (checkIrLedDelay(params) != NO_ERROR)
            CLOGE(" checkIrLedDelay fail");

        if (checkIrLedCurrent(params) != NO_ERROR)
            CLOGE(" checkIrLedCurrent fail");

        if (checkIrLedOnTime(params) != NO_ERROR)
            CLOGE(" checkIrLedOnTime fail");
    }
#ifdef USE_ODC_CAPTURE
    if (checkODCCaptureMode() != NO_ERROR)
        CLOGE(" checkODCCaptureMode fail");
#endif
    return ret;
}

void ExynosCamera1Parameters::setDefaultParameter(void)
{
    CLOGI("");

    status_t ret = NO_ERROR;
    CameraParameters p;
    String8 tempStr;
    char strBuf[256];

    m_cameraInfo.autoFocusMacroPosition = ExynosCameraActivityAutofocus::AUTOFOCUS_MACRO_POSITION_BASE;

    /* Preview Size */
    getMaxPreviewSize(&m_cameraInfo.previewW, &m_cameraInfo.previewH);
    m_setHwPreviewSize(m_cameraInfo.previewW, m_cameraInfo.previewH);

    tempStr.setTo("");
    if (getResolutionList(tempStr, m_staticInfo, &m_cameraInfo.previewW, &m_cameraInfo.previewH, MODE_PREVIEW, m_cameraId) != NO_ERROR) {
        CLOGE("getResolutionList(MODE_PREVIEW) fail");

        m_cameraInfo.previewW = 640;
        m_cameraInfo.previewH = 480;
        tempStr = String8::format("%dx%d", m_cameraInfo.previewW, m_cameraInfo.previewH);
    }

    p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_SIZES, tempStr.c_str());
    CLOGD(" Default preview size is %dx%d", m_cameraInfo.previewW, m_cameraInfo.previewH);
    p.setPreviewSize(m_cameraInfo.previewW, m_cameraInfo.previewH);

    /* Preview Format */
    tempStr.setTo("");
    tempStr = String8::format("%s,%s", CameraParameters::PIXEL_FORMAT_YUV420SP, CameraParameters::PIXEL_FORMAT_YUV420P);
    p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FORMATS, tempStr);
    p.setPreviewFormat(CameraParameters::PIXEL_FORMAT_YUV420SP);


    /* Video Size */
    getMaxVideoSize(&m_cameraInfo.maxVideoW, &m_cameraInfo.maxVideoH);

    tempStr.setTo("");
    if (getResolutionList(tempStr, m_staticInfo, &m_cameraInfo.maxVideoW, &m_cameraInfo.maxVideoH, MODE_VIDEO, m_cameraId) != NO_ERROR) {
        CLOGE("getResolutionList(MODE_VIDEO) fail");

        m_cameraInfo.videoW = 640;
        m_cameraInfo.videoH = 480;
        tempStr = String8::format("%dx%d", m_cameraInfo.maxVideoW, m_cameraInfo.maxVideoH);
    }
#ifdef CAMERA_GED_FEATURE
    else {
#ifdef USE_WQHD_RECORDING
        if (m_addHiddenResolutionList(tempStr, m_staticInfo, 2560, 1440, MODE_VIDEO, m_cameraId) != NO_ERROR) {
            CLOGW("getResolutionList(MODE_VIDEO) fail");
        }
#endif
#ifdef USE_UHD_RECORDING
        if (m_addHiddenResolutionList(tempStr, m_staticInfo, 3840, 2160, MODE_VIDEO, m_cameraId) != NO_ERROR) {
            CLOGW("getResolutionList(MODE_VIDEO) fail");
        }
#endif
    }
#endif

    CLOGD(" KEY_SUPPORTED_VIDEO_SIZES %s", tempStr.c_str());

    p.set(CameraParameters::KEY_SUPPORTED_VIDEO_SIZES, tempStr.c_str());

    CLOGD(" Max video size is %dx%d", m_cameraInfo.maxVideoW, m_cameraInfo.maxVideoH);
    CLOGD(" Default video size is %dx%d", m_cameraInfo.videoW, m_cameraInfo.videoH);
    p.setVideoSize(m_cameraInfo.videoW, m_cameraInfo.videoH);

    /* Video Format */
    if (getAdaptiveCSCRecording() == true) {
        CLOGI("video_frame_foramt == YUV420SP_NV21");
        p.set(CameraParameters::KEY_VIDEO_FRAME_FORMAT, "nv21");
    } else
    {
        CLOGI("video_frame_foramt == YUV420SP");
        p.set(CameraParameters::KEY_VIDEO_FRAME_FORMAT, CameraParameters::PIXEL_FORMAT_YUV420SP);
    }

    /* Preferred preview size for Video */
    tempStr.setTo("");
    tempStr = String8::format("%dx%d", m_cameraInfo.previewW, m_cameraInfo.previewH);
    p.set(CameraParameters::KEY_PREFERRED_PREVIEW_SIZE_FOR_VIDEO, tempStr.c_str());

    /* Picture Size */
    getMaxPictureSize(&m_cameraInfo.pictureW, &m_cameraInfo.pictureH);

    tempStr.setTo("");
    if (getResolutionList(tempStr, m_staticInfo, &m_cameraInfo.pictureW, &m_cameraInfo.pictureH, MODE_PICTURE, m_cameraId) != NO_ERROR) {
        CLOGE("m_getResolutionList(MODE_PICTURE) fail");

        m_cameraInfo.pictureW = 640;
        m_cameraInfo.pictureH = 480;
        tempStr = String8::format("%dx%d", m_cameraInfo.pictureW, m_cameraInfo.pictureH);
    }

    p.set(CameraParameters::KEY_SUPPORTED_PICTURE_SIZES, tempStr.c_str());
    CLOGD(" Default picture size is %dx%d", m_cameraInfo.pictureW, m_cameraInfo.pictureH);
    p.setPictureSize(m_cameraInfo.pictureW, m_cameraInfo.pictureH);

#ifdef SAMSUNG_TN_FEATURE
    tempStr = String8::format("%s,%s,%s", CameraParameters::PIXEL_FORMAT_JPEG,
                                          CameraParameters::PIXEL_FORMAT_YUV420SP,
                                          CameraParameters::PIXEL_FORMAT_YUV420SP_NV21);
#else
    tempStr = String8::format("%s,%s", CameraParameters::PIXEL_FORMAT_JPEG,
                                          CameraParameters::PIXEL_FORMAT_YUV420SP);
#endif

#ifdef SAMSUNG_DNG
    /* DNG Picture Size */
    int dngPictureW = 0, dngPictureH = 0;
    getMaxSensorSize(&dngPictureW, &dngPictureH);

    tempStr.setTo("");
    if (getResolutionList(tempStr, m_staticInfo, &dngPictureW, &dngPictureH, MODE_DNG_PICTURE, m_cameraId) != NO_ERROR) {
        CLOGI("INFO(%s): DNG Picture List is empty.", __FUNCTION__);
    } else {
        p.set("dng-picture-size-values", tempStr.c_str());
        CLOGD("DEBUG(%s): Default dng picture size is %dx%d", __FUNCTION__, dngPictureW, dngPictureH);
    }
#endif

    /* Picture Format */
    p.set(CameraParameters::KEY_SUPPORTED_PICTURE_FORMATS, CameraParameters::PIXEL_FORMAT_JPEG);
    p.setPictureFormat(CameraParameters::PIXEL_FORMAT_JPEG);

    /* Jpeg Quality */
    p.set(CameraParameters::KEY_JPEG_QUALITY, "96"); /* maximum quality */

    /* Thumbnail Size */
    getMaxThumbnailSize(&m_cameraInfo.thumbnailW, &m_cameraInfo.thumbnailH);

    tempStr.setTo("");
    if (getResolutionList(tempStr, m_staticInfo, &m_cameraInfo.thumbnailW, &m_cameraInfo.thumbnailH, MODE_THUMBNAIL, m_cameraId) != NO_ERROR) {
        tempStr = String8::format("%dx%d", m_cameraInfo.thumbnailW, m_cameraInfo.thumbnailH);
    }
    p.set(CameraParameters::KEY_SUPPORTED_JPEG_THUMBNAIL_SIZES, tempStr.c_str());
    p.set(CameraParameters::KEY_JPEG_THUMBNAIL_WIDTH,  m_cameraInfo.thumbnailW);
    p.set(CameraParameters::KEY_JPEG_THUMBNAIL_HEIGHT, m_cameraInfo.thumbnailH);

    /* Thumbnail Quality */
    p.set(CameraParameters::KEY_JPEG_THUMBNAIL_QUALITY, "100");

    /* Exposure */
    p.set(CameraParameters::KEY_MIN_EXPOSURE_COMPENSATION, getMinExposureCompensation());
    p.set(CameraParameters::KEY_MAX_EXPOSURE_COMPENSATION, getMaxExposureCompensation());
    p.set(CameraParameters::KEY_EXPOSURE_COMPENSATION, 0);
    p.setFloat(CameraParameters::KEY_EXPOSURE_COMPENSATION_STEP, getExposureCompensationStep());

    /* Auto Exposure Lock supported */
    if (getAutoExposureLockSupported() == true)
        p.set(CameraParameters::KEY_AUTO_EXPOSURE_LOCK_SUPPORTED, "true");
    else
        p.set(CameraParameters::KEY_AUTO_EXPOSURE_LOCK_SUPPORTED, "false");

    /* Face Detection */
    p.set(CameraParameters::KEY_MAX_NUM_DETECTED_FACES_HW, getMaxNumDetectedFaces());
    p.set(CameraParameters::KEY_MAX_NUM_DETECTED_FACES_SW, 0);

    /* Video Sanptshot Supported */
    if (getVideoSnapshotSupported() == true)
        p.set(CameraParameters::KEY_VIDEO_SNAPSHOT_SUPPORTED, "true");
    else
        p.set(CameraParameters::KEY_VIDEO_SNAPSHOT_SUPPORTED, "false");

    /* Video Stabilization Supported */
    if (getVideoStabilizationSupported() == true)
        p.set(CameraParameters::KEY_VIDEO_STABILIZATION_SUPPORTED, "true");
    else
        p.set(CameraParameters::KEY_VIDEO_STABILIZATION_SUPPORTED, "false");

    /* Focus Mode */
    int focusMode = getSupportedFocusModes();
    tempStr.setTo("");
    if (focusMode & FOCUS_MODE_AUTO) {
        tempStr.append(CameraParameters::FOCUS_MODE_AUTO);
    } else if (focusMode & FOCUS_MODE_FIXED){
        tempStr.append(CameraParameters::FOCUS_MODE_FIXED);
    }
    if (focusMode & FOCUS_MODE_INFINITY) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FOCUS_MODE_INFINITY);
    }
    if (focusMode & FOCUS_MODE_MACRO) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FOCUS_MODE_MACRO);
    }
    if (focusMode & FOCUS_MODE_EDOF) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FOCUS_MODE_EDOF);
    }
    if (focusMode & FOCUS_MODE_CONTINUOUS_VIDEO) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FOCUS_MODE_CONTINUOUS_VIDEO);
    }
    if (focusMode & FOCUS_MODE_CONTINUOUS_PICTURE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE);
    }
    if (focusMode & FOCUS_MODE_CONTINUOUS_PICTURE_MACRO) {
        tempStr.append(",");
        tempStr.append("continuous-picture-macro");
    }
#ifdef SAMSUNG_FIXED_FACE_FOCUS
    if (focusMode & FOCUS_MODE_FIXED_FACE) {
        tempStr.append(",");
        tempStr.append("fixed-face");
    }
#endif

    p.set(CameraParameters::KEY_SUPPORTED_FOCUS_MODES,
          tempStr.c_str());

    if (focusMode & FOCUS_MODE_AUTO)
        p.set(CameraParameters::KEY_FOCUS_MODE,
              CameraParameters::FOCUS_MODE_AUTO);
    else if (focusMode & FOCUS_MODE_CONTINUOUS_PICTURE)
        p.set(CameraParameters::KEY_FOCUS_MODE,
              CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE);
    else if (focusMode & FOCUS_MODE_CONTINUOUS_VIDEO)
        p.set(CameraParameters::KEY_FOCUS_MODE,
              CameraParameters::FOCUS_MODE_CONTINUOUS_VIDEO);
    else if (focusMode & FOCUS_MODE_FIXED)
        p.set(CameraParameters::KEY_FOCUS_MODE,
          CameraParameters::FOCUS_MODE_FIXED);
    else
        p.set(CameraParameters::KEY_FOCUS_MODE,
          CameraParameters::FOCUS_MODE_INFINITY);

/*TODO: This values will be changed */
#define BACK_CAMERA_AUTO_FOCUS_DISTANCES_STR       "0.10,1.20,Infinity"
#define FRONT_CAMERA_FOCUS_DISTANCES_STR           "0.20,0.25,Infinity"

#define BACK_CAMERA_MACRO_FOCUS_DISTANCES_STR      "0.10,0.20,Infinity"
#define BACK_CAMERA_INFINITY_FOCUS_DISTANCES_STR   "0.10,1.20,Infinity"

    /* Focus Distances */
    if (getCameraId() == CAMERA_ID_BACK)
        p.set(CameraParameters::KEY_FOCUS_DISTANCES,
              BACK_CAMERA_AUTO_FOCUS_DISTANCES_STR);
    else
        p.set(CameraParameters::KEY_FOCUS_DISTANCES,
              FRONT_CAMERA_FOCUS_DISTANCES_STR);

    p.set(CameraParameters::FOCUS_DISTANCE_INFINITY, "Infinity");

    /* Max number of Focus Areas */
    p.set(CameraParameters::KEY_MAX_NUM_FOCUS_AREAS, 0);
    if (focusMode & FOCUS_MODE_TOUCH) {
        p.set(CameraParameters::KEY_MAX_NUM_FOCUS_AREAS, 1);
        p.set(CameraParameters::KEY_FOCUS_AREAS, "(0,0,0,0,0)");
    }

    /* Flash */
    int flashMode = getSupportedFlashModes();
    tempStr.setTo("");
    if (flashMode & FLASH_MODE_OFF) {
        tempStr.append(CameraParameters::FLASH_MODE_OFF);
    }
    if (flashMode & FLASH_MODE_AUTO) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FLASH_MODE_AUTO);
    }
    if (flashMode & FLASH_MODE_ON) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FLASH_MODE_ON);
    }
    if (flashMode & FLASH_MODE_RED_EYE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FLASH_MODE_RED_EYE);
    }
    if (flashMode & FLASH_MODE_TORCH) {
        tempStr.append(",");
        tempStr.append(CameraParameters::FLASH_MODE_TORCH);
    }

    p.set(CameraParameters::KEY_SUPPORTED_FLASH_MODES, tempStr.c_str());
    p.set(CameraParameters::KEY_FLASH_MODE, CameraParameters::FLASH_MODE_OFF);

    /* shot-mode */
    p.set("shot-mode", SHOT_MODE_NORMAL);

    /* scene mode */
    int sceneMode = getSupportedSceneModes();
    tempStr.setTo("");
    if (sceneMode & SCENE_MODE_AUTO) {
        tempStr.append(CameraParameters::SCENE_MODE_AUTO);
    }
    if (sceneMode & SCENE_MODE_ACTION) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_ACTION);
    }
    if (sceneMode & SCENE_MODE_PORTRAIT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_PORTRAIT);
    }
    if (sceneMode & SCENE_MODE_LANDSCAPE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_LANDSCAPE);
    }
    if (sceneMode & SCENE_MODE_NIGHT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_NIGHT);
    }
    if (sceneMode & SCENE_MODE_NIGHT_PORTRAIT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_NIGHT_PORTRAIT);
    }
    if (sceneMode & SCENE_MODE_THEATRE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_THEATRE);
    }
    if (sceneMode & SCENE_MODE_BEACH) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_BEACH);
    }
    if (sceneMode & SCENE_MODE_SNOW) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_SNOW);
    }
    if (sceneMode & SCENE_MODE_SUNSET) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_SUNSET);
    }
    if (sceneMode & SCENE_MODE_STEADYPHOTO) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_STEADYPHOTO);
    }
    if (sceneMode & SCENE_MODE_FIREWORKS) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_FIREWORKS);
    }
    if (sceneMode & SCENE_MODE_SPORTS) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_SPORTS);
    }
    if (sceneMode & SCENE_MODE_PARTY) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_PARTY);
    }
    if (sceneMode & SCENE_MODE_CANDLELIGHT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_CANDLELIGHT);
    }
#ifdef SAMSUNG_COMPANION
    if (sceneMode & SCENE_MODE_HDR) {
        tempStr.append(",");
        tempStr.append(CameraParameters::SCENE_MODE_HDR);
    }
#endif

    p.set(CameraParameters::KEY_SUPPORTED_SCENE_MODES,
          tempStr.c_str());
    p.set(CameraParameters::KEY_SCENE_MODE,
          CameraParameters::SCENE_MODE_AUTO);

    /* effect */
    int effect = getSupportedColorEffects();
    tempStr.setTo("");
    if (effect & EFFECT_NONE) {
        tempStr.append(CameraParameters::EFFECT_NONE);
    }
    if (effect & EFFECT_MONO) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_MONO);
    }
    if (effect & EFFECT_NEGATIVE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_NEGATIVE);
    }
    if (effect & EFFECT_SOLARIZE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_SOLARIZE);
    }
    if (effect & EFFECT_SEPIA) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_SEPIA);
    }
    if (effect & EFFECT_POSTERIZE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_POSTERIZE);
    }
    if (effect & EFFECT_WHITEBOARD) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_WHITEBOARD);
    }
    if (effect & EFFECT_BLACKBOARD) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_BLACKBOARD);
    }
    if (effect & EFFECT_AQUA) {
        tempStr.append(",");
        tempStr.append(CameraParameters::EFFECT_AQUA);
    }

    p.set(CameraParameters::KEY_SUPPORTED_EFFECTS, tempStr.c_str());
    p.set(CameraParameters::KEY_EFFECT, CameraParameters::EFFECT_NONE);

    /* white balance */
    int whiteBalance = getSupportedWhiteBalance();
    tempStr.setTo("");
    if (whiteBalance & WHITE_BALANCE_AUTO) {
        tempStr.append(CameraParameters::WHITE_BALANCE_AUTO);
    }
    if (whiteBalance & WHITE_BALANCE_INCANDESCENT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::WHITE_BALANCE_INCANDESCENT);
    }
    if (whiteBalance & WHITE_BALANCE_FLUORESCENT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::WHITE_BALANCE_FLUORESCENT);
    }
    if (whiteBalance & WHITE_BALANCE_WARM_FLUORESCENT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::WHITE_BALANCE_WARM_FLUORESCENT);
    }
    if (whiteBalance & WHITE_BALANCE_DAYLIGHT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::WHITE_BALANCE_DAYLIGHT);
    }
    if (whiteBalance & WHITE_BALANCE_CLOUDY_DAYLIGHT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::WHITE_BALANCE_CLOUDY_DAYLIGHT);
    }
    if (whiteBalance & WHITE_BALANCE_TWILIGHT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::WHITE_BALANCE_TWILIGHT);
    }
    if (whiteBalance & WHITE_BALANCE_SHADE) {
        tempStr.append(",");
        tempStr.append(CameraParameters::WHITE_BALANCE_SHADE);
    }

    p.set(CameraParameters::KEY_SUPPORTED_WHITE_BALANCE,
          tempStr.c_str());
    p.set(CameraParameters::KEY_WHITE_BALANCE, CameraParameters::WHITE_BALANCE_AUTO);

    /* Auto Whitebalance Lock supported */
    if (getAutoWhiteBalanceLockSupported() == true)
        p.set(CameraParameters::KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED, "true");
    else
        p.set(CameraParameters::KEY_AUTO_WHITEBALANCE_LOCK_SUPPORTED, "false");

    /*  anti banding */
    tempStr.setTo("");
    int antiBanding = getSupportedAntibanding();
#ifdef USE_CSC_FEATURE
    /* The Supported List always includes the AUTO Mode - Google */
    if (antiBanding & ANTIBANDING_AUTO) {
        tempStr.append(CameraParameters::ANTIBANDING_AUTO);
    }

    m_chooseAntiBandingFrequency();
    /* 50Hz or 60Hz */
    tempStr.append(",");
    tempStr.append(m_antiBanding);
#else

    if (antiBanding & ANTIBANDING_AUTO) {
        tempStr.append(CameraParameters::ANTIBANDING_AUTO);
    }
    if (antiBanding & ANTIBANDING_50HZ) {
        tempStr.append(",");
        tempStr.append(CameraParameters::ANTIBANDING_50HZ);
    }
    if (antiBanding & ANTIBANDING_60HZ) {
        tempStr.append(",");
        tempStr.append(CameraParameters::ANTIBANDING_60HZ);
    }
    if (antiBanding & ANTIBANDING_OFF) {
        tempStr.append(",");
        tempStr.append(CameraParameters::ANTIBANDING_OFF);
    }
#endif

    p.set(CameraParameters::KEY_SUPPORTED_ANTIBANDING,
          tempStr.c_str());

#ifdef USE_CSC_FEATURE
    p.set(CameraParameters::KEY_ANTIBANDING, m_antiBanding);
#else
    p.set(CameraParameters::KEY_ANTIBANDING, CameraParameters::ANTIBANDING_AUTO);
#endif

    /* rotation */
    p.set(CameraParameters::KEY_ROTATION, 0);

    /* view angle */
    setHorizontalViewAngle(640, 480);
    p.setFloat(CameraParameters::KEY_HORIZONTAL_VIEW_ANGLE, getHorizontalViewAngle());
    p.setFloat(CameraParameters::KEY_VERTICAL_VIEW_ANGLE, getVerticalViewAngle());

    /* metering */
    p.set(CameraParameters::KEY_MAX_NUM_METERING_AREAS, getMaxNumMeteringAreas());
    p.set(CameraParameters::KEY_METERING_AREAS, 0);

    /* zoom */
    if (getZoomSupported() == true) {
        int maxZoom = getMaxZoomLevel();
        if (0 < maxZoom) {
            p.set(CameraParameters::KEY_ZOOM_SUPPORTED, "true");

            if (getSmoothZoomSupported() == true)
                p.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "true");
            else
                p.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "false");

            p.set(CameraParameters::KEY_MAX_ZOOM, maxZoom - 1);
            p.set(CameraParameters::KEY_ZOOM, ZOOM_LEVEL_0);

            int max_zoom_ratio = (int)getMaxZoomRatio();
            tempStr.setTo("");
            if (getZoomRatioList(tempStr, maxZoom, max_zoom_ratio, m_staticInfo->zoomRatioList) == NO_ERROR)
                p.set(CameraParameters::KEY_ZOOM_RATIOS, tempStr.c_str());
            else
                p.set(CameraParameters::KEY_ZOOM_RATIOS, "100");

            p.set("constant-growth-rate-zoom-supported", "true");

            CLOGI("[setDefaultParameter]maxZoom=%d, max_zoom_ratio= %d, zoomRatioList=%s", maxZoom, max_zoom_ratio, tempStr.c_str());
        } else {
            p.set(CameraParameters::KEY_ZOOM_SUPPORTED, "false");
            p.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "false");
            p.set(CameraParameters::KEY_MAX_ZOOM, ZOOM_LEVEL_0);
            p.set(CameraParameters::KEY_ZOOM, ZOOM_LEVEL_0);
        }
    } else {
        p.set(CameraParameters::KEY_ZOOM_SUPPORTED, "false");
        p.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "false");
        p.set(CameraParameters::KEY_MAX_ZOOM, ZOOM_LEVEL_0);
        p.set(CameraParameters::KEY_ZOOM, ZOOM_LEVEL_0);
    }

    /* fps */
    uint32_t minFpsRange = 7;
    uint32_t maxFpsRange = 30;

    getPreviewFpsRange(&minFpsRange, &maxFpsRange);
#ifdef TEST_GED_HIGH_SPEED_RECORDING
    maxFpsRange = 120;
#endif
    CLOGI("[getPreviewFpsRange]minFpsRange=%d, maxFpsRange=%d", (int)minFpsRange, (int)maxFpsRange);
    int minFps = (minFpsRange == 0) ? 0 : (int)minFpsRange;
    int maxFps = (maxFpsRange == 0) ? 0 : (int)maxFpsRange;

    tempStr.setTo("");
    snprintf(strBuf, 256, "%d", minFps);
    tempStr.append(strBuf);

    for (int i = minFps + 1; i <= maxFps; i++) {
        snprintf(strBuf, 256, ",%d", i);
        tempStr.append(strBuf);
    }
    p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FRAME_RATES,  tempStr.c_str());

    minFpsRange = minFpsRange * 1000;
    maxFpsRange = maxFpsRange * 1000;

    tempStr.setTo("");
    getSupportedFpsList(tempStr, minFpsRange, maxFpsRange, m_cameraId, m_staticInfo);
    CLOGI("[setDefaultParameter]supportedFpsList=%s", tempStr.c_str());
    p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FPS_RANGE, tempStr.c_str());
    /* p.set(CameraParameters::KEY_SUPPORTED_PREVIEW_FPS_RANGE, "(15000,30000),(30000,30000)"); */

    /* limit 30 fps on default setting. */
    if (30 < maxFps)
        maxFps = 30;
    p.setPreviewFrameRate(maxFps);

    if (30000 < maxFpsRange)
        maxFpsRange = 30000;
    snprintf(strBuf, 256, "%d,%d", maxFpsRange/2, maxFpsRange);
    p.set(CameraParameters::KEY_PREVIEW_FPS_RANGE, strBuf);

    /* focal length */
    int num = 0;
    int den = 0;
    int precision = 0;
    getFocalLength(&num, &den);

    switch (den) {
    default:
    case 1000:
        precision = 3;
        break;
    case 100:
        precision = 2;
        break;
    case 10:
        precision = 1;
        break;
    case 1:
        precision = 0;
        break;
    }
    snprintf(strBuf, 256, "%.*f", precision, ((float)num / (float)den));
    p.set(CameraParameters::KEY_FOCAL_LENGTH, strBuf);

    /* Additional params. */
    p.set("contrast", "auto");
    p.set("iso", "auto");

    // Set supported ISO values
    tempStr.setTo("");
#ifdef SAMSUNG_TN_FEATURE
    int isoValues = getSupportedISO();
    if (isoValues & ISO_AUTO) {
        tempStr.append(CameraParameters::ISO_AUTO);
    }
    if (isoValues & ISO_100) {
        tempStr.append(",");
        tempStr.append(CameraParameters::ISO_100);
    }
    if (isoValues & ISO_200) {
        tempStr.append(",");
        tempStr.append(CameraParameters::ISO_200);
    }
    if (isoValues & ISO_400) {
        tempStr.append(",");
        tempStr.append(CameraParameters::ISO_400);
    }
    if (isoValues & ISO_800) {
        tempStr.append(",");
        tempStr.append(CameraParameters::ISO_800);
    }
#endif
    p.set("iso-values",
          tempStr.c_str());

    p.set("wdr", 0);
    p.set("hdr-mode", 0);
    p.set("metering", "center");    /* For Samsung SDK */

#ifdef SAMSUNG_TN_FEATURE
    // Set Supported Metering Mode
    int meteringModes = getSupportedMeteringMode();
    tempStr.setTo("");
    if (meteringModes & METERING_MODE_MATRIX) {
        tempStr.append(CameraParameters::METERING_MATRIX);
    }
    if (meteringModes & METERING_MODE_CENTER) {
        tempStr.append(",");
        tempStr.append(CameraParameters::METERING_CENTER);
    }
    if (meteringModes & METERING_MODE_SPOT) {
        tempStr.append(",");
        tempStr.append(CameraParameters::METERING_SPOT);
    }

    /* For Samsung SDK */
    p.set("metering-values", tempStr.c_str());
#endif

    p.set("brightness", 0);
    p.set("brightness-max", 2);
    p.set("brightness-min", -2);

    p.set("saturation", 0);
    p.set("saturation-max", 2);
    p.set("saturation-min", -2);

    p.set("sharpness", 0);
    p.set("sharpness-max", 2);
    p.set("sharpness-min", -2);

    p.set("hue", 0);
    p.set("hue-max", 2);
    p.set("hue-min", -2);

    /* For Series shot */
    p.set("burst-capture", 0);
    p.set("best-capture", 0);

    /* fnumber */
    getFnumber(&num, &den);
    p.set("fnumber-value-numerator", num);
    p.set("fnumber-value-denominator", den);

    /* max aperture value */
    getApertureValue(&num, &den);
    p.set("maxaperture-value-numerator", num);
    p.set("maxaperture-value-denominator", den);

    /* focal length */
    getFocalLength(&num, &den);
    p.set("focallength-value-numerator", num);
    p.set("focallength-value-denominator", den);

    /* focal length in 35mm film */
    int focalLengthIn35mmFilm = 0;
    focalLengthIn35mmFilm = getFocalLengthIn35mmFilm();
    p.set("focallength-35mm-value", focalLengthIn35mmFilm);

#if defined(USE_3RD_BLACKBOX) /* KOR ONLY */
    /* scale mode */
    bool supportedScalableMode = getSupportedScalableSensor();
    if (supportedScalableMode == true)
        p.set("scale_mode", -1);
#endif

#if defined(TEST_APP_HIGH_SPEED_RECORDING)
    p.set("fast-fps-mode", 0);
#endif

#ifdef SAMSUNG_LLV
    /* LLV */
    p.set("llv_mode", 0);
#endif

#ifdef SAMSUNG_COMPANION
#ifdef SAMSUNG_TN_FEATURE
    p.set(CameraParameters::KEY_DYNAMIC_RANGE_CONTROL, "off");
    /* PHASE_AF */
    if (getCameraId() == CAMERA_ID_BACK || getCameraId() == CAMERA_ID_BACK_1) {
        p.set(CameraParameters::KEY_SUPPORTED_PHASE_AF, "off,on");  /* For Samsung SDK */
        p.set(CameraParameters::KEY_PHASE_AF, "on");
    } else {
        p.set(CameraParameters::KEY_SUPPORTED_PHASE_AF, "off");  /* For Samsung SDK */
        p.set(CameraParameters::KEY_PHASE_AF, "off");
    }

    /* RT_HDR */
    p.set(CameraParameters::KEY_SUPPORTED_RT_HDR, "off,on,auto"); /* For Samsung SDK */
    p.set(CameraParameters::KEY_RT_HDR, "off");
#endif
#else
    /* PHASE_AF */
#ifdef SAMSUNG_TN_FEATURE
    p.set(CameraParameters::KEY_DYNAMIC_RANGE_CONTROL, "off");
    p.set(CameraParameters::KEY_SUPPORTED_PHASE_AF, "off"); /* For Samsung SDK */
    p.set(CameraParameters::KEY_PHASE_AF, "off");
#endif

    /* RT_HDR */
#ifdef SAMSUNG_TN_FEATURE
    p.set(CameraParameters::KEY_SUPPORTED_RT_HDR, "off"); /* For Samsung SDK */
    p.set(CameraParameters::KEY_RT_HDR, "off");
#endif
#endif
    p.set("imageuniqueid-value", 0);

#if defined(SAMSUNG_OIS)
    p.set("ois-supported", "true");
    p.set("ois", "still");
#else
    //p.set("ois-supported", "false");
#endif

#ifdef SAMSUNG_VRMODE
    p.set("vrmode-supported", "true");
#endif

#if defined(BURST_CAPTURE) && defined(VARIABLE_BURST_FPS)
    tempStr.setTo("");
    tempStr = String8::format("(%d,%d)", BURSTSHOT_MIN_FPS, BURSTSHOT_MAX_FPS);
    p.set("burstshot-fps-values", tempStr.c_str());
#else
    p.set("burstshot-fps-values", "(0,0)");
#endif

    p.set("drc", "false");
    p.set("3dnr", "false");
    p.set("odc", "false");
#ifdef USE_FADE_IN_ENTRANCE
    p.set("entrance", "false");
#endif

#ifdef SAMSUNG_MANUAL_FOCUS
    p.set("focus-distance", -1);
#endif
#ifdef SAMSUNG_QUICKSHOT
    p.set("quick-shot", 0);
#endif
    p.set("effectrecording-hint", 0);

    /* Exposure Time */
    if (getMinExposureTime() != 0 && getMaxExposureTime() != 0) {
        p.set("min-exposure-time", getMinExposureTime());
        p.set("max-exposure-time", getMaxExposureTime());
    }
    p.set("exposure-time", 0);

    /* WB Kelvin */
    if (getMinWBK() != 0 && getMaxWBK() != 0) {
        p.set("min-wb-k", getMinWBK());
        p.set("max-wb-k", getMaxWBK());
    }
    p.set("wb-k", 0);

    /* Limitation time of recording
    * 0 or Not declared : No limit
    * seconds           : Limit for seconds (ex. 600 : 10 minute)
    */
    p.set("max-limit-recording-time-fhd60", 600);
    //p.set("max-limit-recording-time-slowmotion", 600);  // remove, since actual available time is less than 10 min
    p.set("max-limit-recording-time-uhd", 600);
    //p.set("max-limit-recording-time-wqhd", 600);  // remove, since there is no temperature issue in wqhd video recording.

#ifdef SAMSUNG_LENS_DC
    if (getCameraId() == CAMERA_ID_BACK) {
        p.set("rear-lens-distortion-correction", "on");
    } else {
        p.set("rear-lens-distortion-correction", "off");
    }
#endif

#ifdef USE_GPU_EFFECT_FPS_RANGE
    tempStr.setTo("");
    tempStr = String8::format("(%d,%d)", GPU_EFFECT_FPS_MIN, GPU_EFFECT_FPS_MAX);
    p.set("effect-available-fps-values", tempStr.c_str());
#else
    p.set("effect-available-fps-values", "(0,0)");
#endif

    /* set Parameter for SECURE Camera */
    if (m_scenario == SCENARIO_SECURE) {
        p.set("min-gain", m_staticInfo->minGain);
        p.set("max-gain", m_staticInfo->maxGain);
        p.set("gain", m_gain);                 //     2.0

        p.set("min-shutterspeed", m_staticInfo->minShutterSpeed);
        p.set("max-shutterspeed", m_staticInfo->maxShutterSpeed);
        p.set("shutterspeed", m_shutterSpeed); //  30.0ms

        p.set("min-dac", m_staticInfo->minDac);
        p.set("max-dac", m_staticInfo->maxDac);
        p.set("dac", m_irLedCurrent);          //  1050mA

        p.set("min-pulse-delay", m_staticInfo->minPulseDelay);
        p.set("max-pulse-delay", m_staticInfo->maxPulseDelay);
        p.set("pulse-delay", m_irLedDelay);    //   0.0ms

        p.set("min-pulse-width", m_staticInfo->minPulseWidth);
        p.set("max-pulse-width", m_staticInfo->maxPulseWidth);
        p.set("pulse-width", m_irLedWidth);    //  10.0ms

        p.set("min-led-max-time", m_staticInfo->minLedMaxTime);
        p.set("max-led-max-time", m_staticInfo->maxLedMaxTime);
        p.set("led-max-time", m_irLedOnTime);  //     12s
    }

#ifdef USE_CP_FUSION_LIB
    m_fusionCalData = readFusionCalData(&m_fusionCalDataSize);
    if (m_fusionCalData == NULL) {
        CLOGE("readFusionCalData() fail");
    } else {
        setFusionCalData(m_fusionCalData, m_fusionCalDataSize);
    }
#endif
    //VDIS Video crop infomation
#ifdef SUPPORT_SW_VDIS
    int nVideoW, nVideoH;
    int vdisW, vdisH;
    double cropRatio = 0;

    nVideoW = 3840, nVideoH = 2160;
    vdisW = 4032, vdisH = 2268;
    cropRatio = (double)vdisW / (double) nVideoW;
    cropRatio = ((cropRatio - 1.0) * 100 + 0.5);
    p.set("vdis-crop-ratio_3840x2160",(int)cropRatio);

    nVideoW = 2560, nVideoH = 1440;
    vdisW = 3072, vdisH = 1728;
    cropRatio = (double)vdisW / (double) nVideoW;
    cropRatio = ((cropRatio - 1.0) * 100 + 0.5);
    p.set("vdis-crop-ratio_2560x1440",(int)cropRatio);

    nVideoW = 1920, nVideoH = 1080;
    vdisW = 2304, vdisH = 1296;
    cropRatio = (double)vdisW / (double) nVideoW;
    cropRatio = ((cropRatio - 1.0) * 100 + 0.5);
    p.set("vdis-crop-ratio_1920x1080",(int)cropRatio);

    nVideoW = 1280, nVideoH = 720;
    vdisW = 1536, vdisH = 864;
    cropRatio = (double)vdisW / (double) nVideoW;
    cropRatio = ((cropRatio - 1.0) * 100 + 0.5);
    p.set("vdis-crop-ratio_1280x720",(int)cropRatio);
#endif

    m_params = p;

    /* make sure m_secCamera has all the settings we do.  applications
     * aren't required to call setParameters themselves (only if they
     * want to change something.
     */
    ret = setParameters(p);
    if (ret < 0)
        CLOGE("setParameters is fail");
}

status_t ExynosCamera1Parameters::checkVisionMode(const CameraParameters& params)
{
    status_t ret;

    /* Check vision mode */
    int intelligent_mode = params.getInt("intelligent-mode");

    ret = m_setIntelligentMode(intelligent_mode);
    if (ret != NO_ERROR) {
        CLOGE("[setParameters]Inavalid Intelligent mode");
        return ret;
    }
    m_params.set("intelligent-mode", intelligent_mode);

    CLOGD("[setParameters]intelligent_mode(%d) getVisionMode(%d)", intelligent_mode, getVisionMode());

    /* Smart stay need to skip more frames */
    int skipCompensation = m_frameSkipCounter.getCompensation();
    if (intelligent_mode == 1) {
        m_frameSkipCounter.setCompensation(skipCompensation + SMART_STAY_SKIP_COMPENSATION);
    } else {
        m_frameSkipCounter.setCompensation(skipCompensation);
    }

    if (getVisionMode() == true) {
        /* preset for each vision mode */
        switch (intelligent_mode) {
        case 2:
            m_setVisionModeFps(10);
            break;
        case 3:
            m_setVisionModeFps(5);
            break;
        default:
            m_setVisionModeFps(10);
            break;
        }

/* Vision mode custom frame rate will be enabled when application ready */
#if 0
        /* If user wants to set custom fps, vision mode set max fps to frame rate */
        int minFps = -1;
        int maxFps = -1;
        params.getPreviewFpsRange(&minFps, &maxFps);

        if (minFps > 0 && maxFps > 0) {
            CLOGD(" set user frame rate (%d)", maxFps / 1000);
            m_setVisionModeFps(maxFps / 1000);
        }
#endif

        /* smart-screen-exposure */
        int newVisionAeTarget = params.getInt("smart-screen-exposure");
        if (0 < newVisionAeTarget) {
            CLOGD("[setParameters]newVisionAeTarget : %d", newVisionAeTarget);
            m_setVisionModeAeTarget(newVisionAeTarget);
            m_params.set("smart-screen-exposure", newVisionAeTarget);
        }

        return OK;
    } else {
        if (m_scenario == SCENARIO_SECURE) {
            m_setVisionMode(true);
            CLOGD("DEBUG(%s):secure mode enabled.", "setParameters");
        }
        return NO_ERROR;
    }
}

#ifdef USE_FW_OPMODE
status_t ExynosCamera1Parameters::checkFactorytest(const CameraParameters& params)
{
    /* Check factorytest param */
    int factorytest = params.getInt("factorytest");

    if (factorytest == 1)
        setMetaUctlOPMode(&m_metadata, CAMERA_OP_MODE_FAC);

    return NO_ERROR;
}
#endif

status_t ExynosCamera1Parameters::m_setIntelligentMode(int intelligentMode)
{
    status_t ret = NO_ERROR;
    bool visionMode = false;

    m_cameraInfo.intelligentMode = intelligentMode;

    if (intelligentMode > 1) {
        if (m_staticInfo->visionModeSupport == true) {
            visionMode = true;
        } else {
            CLOGE("[setParameters]tried to set vision mode(not supported)");
            ret = BAD_VALUE;
        }
    } else if (getVisionMode()) {
        CLOGW("vision mode can not change before stoppreview");
        visionMode = true;
    }

    m_setVisionMode(visionMode);

    return ret;
 }

int ExynosCamera1Parameters::getIntelligentMode(void)
{
    return m_cameraInfo.intelligentMode;
}

void ExynosCamera1Parameters::m_setVisionMode(bool vision)
{
    m_cameraInfo.visionMode = vision;
}

bool ExynosCamera1Parameters::getVisionMode(void)
{
    return m_cameraInfo.visionMode;
}

void ExynosCamera1Parameters::m_setVisionModeFps(int fps)
{
    m_cameraInfo.visionModeFps = fps;
}

int ExynosCamera1Parameters::getVisionModeFps(void)
{
    return m_cameraInfo.visionModeFps;
}

void ExynosCamera1Parameters::m_setVisionModeAeTarget(int ae)
{
    m_cameraInfo.visionModeAeTarget = ae;
}

int ExynosCamera1Parameters::getVisionModeAeTarget(void)
{
    return m_cameraInfo.visionModeAeTarget;
}

#if defined(SAMSUNG_COMPANION) || defined(SAMSUNG_EEPROM)
void ExynosCamera1Parameters::setRomReadThreadDone(bool enable)
{
    Mutex::Autolock lock(m_parameterLock);

    m_romReadThreadDone = enable;
}
#endif

status_t ExynosCamera1Parameters::checkSWVdisMode(const CameraParameters& params)
{
    const char *newSwVdis = params.get("sw-vdis");
    if (newSwVdis != NULL) {
        CLOGD("[setParameters]newSwVdis %s", newSwVdis);
        bool swVdisMode = true;

        if (!strcmp(newSwVdis, "off"))
            swVdisMode = false;

        m_setSWVdisMode(swVdisMode);
        m_params.set("sw-vdis", newSwVdis);
    }

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::checkVideoSize(const CameraParameters& params)
{
    /*  Video size */
    int newVideoW = 0;
    int newVideoH = 0;
    int newHwVideoW = 0;
    int newHwVideoH = 0;

    params.getVideoSize(&newVideoW, &newVideoH);

    /* size is negative, set default size */
    if ((newVideoW < 0) || (newVideoH < 0)) {
        CLOGW("size(%d x %d) is invalid", newVideoW, newVideoH);

        params.dump();

        newVideoW = 1920;
        newVideoH = 1080;
        CLOGW("set default size as (%d x %d)", newVideoW, newVideoH);
    }

    if (0 < newVideoW && 0 < newVideoH &&
        m_isSupportedVideoSize(newVideoW, newVideoH) == false) {
        return BAD_VALUE;
    }

    /* restart when video size is changed to control MCSC2 */
    int oldVideoW = 0;
    int oldVideoH = 0;

    getVideoSize(&oldVideoW, &oldVideoH);

    if ((oldVideoW != newVideoW) || (oldVideoH != newVideoH)) {
        m_setRestartPreviewChecked(true);
        m_previewSizeChanged = true;
        CLOGE("setRestartPreviewChecked true");
    }

    CLOGI("[setParameters]newVideo Size (%dx%d), ratioId(%d)",
        newVideoW, newVideoH, m_cameraInfo.videoSizeRatioId);
    m_setVideoSize(newVideoW, newVideoH);
    m_params.setVideoSize(newVideoW, newVideoH);

    newHwVideoW = newVideoW;
    newHwVideoH = newVideoH;
    m_setHwVideoSize(newHwVideoW, newHwVideoH);

#ifdef SUPPORT_SW_VDIS
    if (newVideoW == 3840 && newVideoH == 2160) {
        m_swVDIS_UHD = true;
    } else {
        m_swVDIS_UHD = false;
    }
#endif
    return NO_ERROR;
}

status_t ExynosCamera1Parameters::checkHWVdisMode(__unused const CameraParameters& params)
{
    status_t ret = NO_ERROR;

    bool hwVdisMode = this->getHWVdisMode();

    CLOGD("[setParameters]newHwVdis %d", hwVdisMode);

    ret = setDisEnable(hwVdisMode);
    if (ret != NO_ERROR) {
        CLOGE("setDisEnable(%d) fail", hwVdisMode);
    } else {
        if (m_flagHWVDisMode != hwVdisMode) {
            m_flagHWVDisMode = hwVdisMode;
        }
    }

    return ret;
}

status_t ExynosCamera1Parameters::check3dnrBypass(bool is3dnrMode, bool *flagBypass)
{
    int hwVideoW = 0, hwVideoH = 0;
    uint32_t minFpsRange = 0, maxFpsRange = 0;
    bool new3dnrMode = false;

    getHwVideoSize(&hwVideoW, &hwVideoH);
    getPreviewFpsRange(&minFpsRange, &maxFpsRange);

    if (is3dnrMode == true) {
        if (getDualMode() == true
#ifdef USE_DUAL_CAMERA
            && (getDualCameraMode() == false)
#endif
           ) {
            /* 1. TDNR : dual always disabled. */
#if defined(USE_3DNR_DUAL)
            new3dnrMode = USE_3DNR_DUAL;
#else
            new3dnrMode = false;
#endif
        } else if (hwVideoW >= 3840 && hwVideoH >= 2160) {
            /* 2. TDNR : UHD Recording Featuring */
#if defined(USE_3DNR_UHDRECORDING)
            if (minFpsRange < 60 && maxFpsRange < 60)
                new3dnrMode = USE_3DNR_UHDRECORDING;
            else
#endif
                new3dnrMode = false;
        } else if (minFpsRange > 60 || maxFpsRange > 60) {
            /* 3. TDNR : HighSpeed Recording Featuring */
#if defined(USE_3DNR_HIGHSPEED_RECORDING)
            new3dnrMode = USE_3DNR_HIGHSPEED_RECORDING;
#else
            new3dnrMode = false;
#endif
        } else if (getVtMode() > 0) {
            /* 4. TDNR : Vt call Featuring */
#if defined(USE_3DNR_VTCALL)
            new3dnrMode = USE_3DNR_VTCALL;
#else
            new3dnrMode = false;
#endif
        } else if (getIntelligentMode() == 1) {
            /* 5. TDNR : smart stay */
#if defined(USE_3DNR_SMARTSTAY)
            new3dnrMode = USE_3DNR_SMARTSTAY;
#else
            new3dnrMode = false;
#endif
        } else {
#ifdef USE_LIVE_BROADCAST
            if (getPLBMode() == true)
                new3dnrMode = false;
            else
#endif
                new3dnrMode = true;
        }
    } else {
        new3dnrMode = false;
    }

    *flagBypass = !(new3dnrMode);

    return NO_ERROR;
}

bool ExynosCamera1Parameters::isSWVdisMode(void)
{
    bool swVDIS_mode = false;
    bool use3DNR_dmaout = false;
    int configMode = getConfigMode();

    int nVideoW, nVideoH;
    getVideoSize(&nVideoW, &nVideoH);

    if ((getRecordingHint() == true) &&
#ifndef SUPPORT_SW_VDIS_FRONTCAM
        (getCameraId() == CAMERA_ID_BACK) &&
#endif /*SUPPORT_SW_VDIS_FRONTCAM*/
        (configMode == CONFIG_MODE::NORMAL
         || configMode == CONFIG_MODE::HIGHSPEED_60) &&
        (use3DNR_dmaout == false) &&
        (getSWVdisUIMode() == true) &&
        ((nVideoW == 3840 && nVideoH == 2160)
         || (nVideoW == 2560 && nVideoH == 1440)
         || (nVideoW == 1920 && nVideoH == 1080)
         || (nVideoW == 1280 && nVideoH == 720)))
    {
        swVDIS_mode = true;
    }

    return swVDIS_mode;
}

bool ExynosCamera1Parameters::isSWVdisModeWithParam(int nWidth, int nHeight)
{
    bool swVDIS_mode = false;
    bool use3DNR_dmaout = false;
    int configMode = getConfigMode();

    if ((getRecordingHint() == true) &&
#ifndef SUPPORT_SW_VDIS_FRONTCAM
        (getCameraId() == CAMERA_ID_BACK) &&
#endif /*SUPPORT_SW_VDIS_FRONTCAM*/
        (configMode == CONFIG_MODE::NORMAL
         || configMode == CONFIG_MODE::HIGHSPEED_60) &&
        (use3DNR_dmaout == false) &&
        (getSWVdisUIMode() == true) &&
        ((nWidth == 3840 && nHeight == 2160)
         || (nWidth == 2560 && nHeight == 1440)
         || (nWidth == 1920 && nHeight == 1080)
         || (nWidth == 1280 && nHeight == 720)))
    {
        swVDIS_mode = true;
    }

    return swVDIS_mode;
}

bool ExynosCamera1Parameters::isSWVdisOnPreview(void)
{
    bool swVDIS_OnPreview = false;

    if (this->isSWVdisMode() == true) {
        if (getHighSpeedRecording() == false && getCameraId() == CAMERA_ID_BACK) {
            swVDIS_OnPreview = true;
        }
    }

    return swVDIS_OnPreview;
}

bool ExynosCamera1Parameters::getHWVdisMode(void)
{
    bool ret = this->getVideoStabilization();

    /*
     * Only true case,
     * we will test whether support or not.
     */
    if (ret == true) {
        switch (getCameraId()) {
        case CAMERA_ID_BACK:
#ifdef SUPPORT_BACK_HW_VDIS
            ret = SUPPORT_BACK_HW_VDIS;
#else
            ret = false;
#endif
            break;
        case CAMERA_ID_FRONT:
#ifdef SUPPORT_FRONT_HW_VDIS
            ret = SUPPORT_FRONT_HW_VDIS;
#else
            ret = false;
#endif
            break;
        case CAMERA_ID_BACK_1:
#ifdef SUPPORT_BACK_1_HW_VDIS
            ret = SUPPORT_BACK_1_HW_VDIS;
#else
            ret = false;
#endif
            break;
        default:
            ret = false;
            break;
        }
    }

#ifdef SUPPORT_SW_VDIS
    if (ret == true &&
        this->isSWVdisMode() == true) {
        ret = false;
    }
#endif /*SUPPORT_SW_VDIS*/

    return ret;
}

int ExynosCamera1Parameters::getHWVdisFormat(void)
{
    return V4L2_PIX_FMT_YUYV;
}

void ExynosCamera1Parameters::m_setSWVdisMode(bool swVdis)
{
    m_cameraInfo.swVdisMode = swVdis;
}

bool ExynosCamera1Parameters::getSWVdisMode(void)
{
    return m_cameraInfo.swVdisMode;
}

void ExynosCamera1Parameters::m_setSWVdisUIMode(bool swVdisUI)
{
    m_cameraInfo.swVdisUIMode = swVdisUI;
}

bool ExynosCamera1Parameters::getSWVdisUIMode(void)
{
    return m_cameraInfo.swVdisUIMode;
}

#ifdef SAMSUNG_HYPER_MOTION
status_t ExynosCamera1Parameters::checkHyperMotionSpeed(const CameraParameters& params)
{
    int newMotionSpeed = params.getInt("motion-speed");
    int curMotionSpeed = getHyperMotionSpeed();

    if (newMotionSpeed < -1 || newMotionSpeed > 4) {
        CLOGE(" Invalid HyperMotion Speed(%d)", newMotionSpeed);
        return BAD_VALUE;
    }

    CLOGD("[setParameters]newMotionSpeed %d", newMotionSpeed);

    if (curMotionSpeed != newMotionSpeed) {
        m_setHyperMotionSpeed(newMotionSpeed);
        m_params.set("motion-speed", newMotionSpeed);
    } else {
        CLOGD(" No value change in HyperMotion Speed : %d", newMotionSpeed);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setHyperMotionSpeed(int motionSpeed)
{
    m_cameraInfo.hyperMotionSpeed = motionSpeed;
}

int ExynosCamera1Parameters::getHyperMotionSpeed(void)
{
    return m_cameraInfo.hyperMotionSpeed;
}

void ExynosCamera1Parameters::m_isHyperMotionMode(const CameraParameters& params)
{
    bool hyperMotionMode;
    int shotmode = params.getInt("shot-mode");

    if (shotmode == SHOT_MODE_HYPER_MOTION)
        hyperMotionMode = true;
    else
        hyperMotionMode = false;

    CLOGD("[setParameters]hyperMotionMode:%s",
        hyperMotionMode == true? "on":"off");

    m_setHyperMotionMode(hyperMotionMode);
}

void ExynosCamera1Parameters::m_setHyperMotionMode(bool enable)
{
    m_cameraInfo.hyperMotionMode = enable;
}

bool ExynosCamera1Parameters::getHyperMotionMode(void)
{
    return m_cameraInfo.hyperMotionMode;
}

void ExynosCamera1Parameters::getHyperMotionCropSize(int inputW, int inputH, int outputW, int outputH,
                                                        int *cropX, int *corpY, int *cropW, int *cropH)
{
    int hyperMotion_OutW, hyperMotion_OutH;

    hyperMotion_OutW = outputW;
    hyperMotion_OutH = outputH;
    m_hyperMotion_AdjustPreviewSize(&hyperMotion_OutW, &hyperMotion_OutH);

    *cropW = (inputW * hyperMotion_OutW) / outputW;
    *cropH = (inputH * hyperMotion_OutH) / outputH;
    *cropX = (inputW - *cropW) / 2;
    *corpY = (inputH - *cropH) / 2;
}

void ExynosCamera1Parameters::m_getHyperMotionPreviewSize(int w, int h, int *newW, int *newH)
{
    if (w < 0 || h < 0) {
        return;
    }

    if (w == 1920 && h == 1080) {
        *newW = 2304;
        *newH = 1296;
    } else if (w == 1280 && h == 720) {
        *newW = 1536;
        *newH = 864;
    } else {
        *newW = ALIGN_UP((w * 6) / 5, CAMERA_16PX_ALIGN);
        *newH = ALIGN_UP((h * 6) / 5, CAMERA_16PX_ALIGN);
    }
}
#endif

#ifdef SAMSUNG_DUAL_SOLUTION
float ExynosCamera1Parameters::getZoomRatio(int cameraId, int zoomLevel)
{
    float zoomRatio = 1.00f;
    if (getZoomSupported() == true) {
        zoomRatio = (float)m_staticInfo->zoomRatioList[zoomLevel];
        if (m_fusionPreviewWrapper->m_getIsInit() == true && getDualCameraMode() == true) {
            zoomRatio = m_fusionPreviewWrapper->m_getZoomRatio(cameraId, zoomLevel);
        }
    } else
        zoomRatio = 1000.00f;

    return zoomRatio;
}

void ExynosCamera1Parameters::setPreviewNeedMargin(int zoomLevel)
{
    if (m_fusionPreviewWrapper->m_getIsInit() == true && getDualCameraMode() == true) {
        int needMargin = m_fusionPreviewWrapper->m_getNeedMargin(getCameraId(), zoomLevel);
        m_needPreviewMargin = needMargin;
    }

    return;
}

float ExynosCamera1Parameters::getZoomOrgRatio(int zoomLevel)
{
    float zoomRatio = 1.00f;
    if (getZoomSupported() == true)
        zoomRatio = (float)m_staticInfo->zoomRatioList[zoomLevel];
    else
        zoomRatio = 1000.00f;

    return zoomRatio;
}

status_t ExynosCamera1Parameters::adjustDualSolutionSize(int targetWidth, int targetHeight)
{
    int dualSolutionSize[][DUAL_SOLUTION_SIZE_MAX] = {
        /* dstW, dstH, srcWideW, srcWideH, srcTeleW, srcTeleH */
        {1920, 1080, 2304, 1296, 2304, 1296}, /* preview 16:9, FHD */
        {1440, 1080, 1728, 1296, 1728, 1296}, /* preview 4:3 */
        {1088, 1088, 1312, 1312, 1312, 1312}, /* preview 1:1 */
        {2224, 1080, 2672, 1298, 2672, 1298}, /* preview 18.5:9 */
        {3840, 2160, 3840, 2160, 3840, 2160},  /* UHD */
        {2560, 1440, 3072, 1728, 3072, 1728},  /* QHD */
        {1280, 720,  1536, 864,  1536, 864},  /* HD */
        {960,  720,  1152, 864,  1152, 864},  /* preview 4:3 */
        {640,  480,  768,  576,  768,  576},  /* VGA */
    };

    int sizeListLen = 0;

#ifdef SUPPORT_SW_VDIS
    int dualSolutionVdisSize[][DUAL_SOLUTION_SIZE_MAX] = {
        /* dstW, dstH, srcWideW, srcWideH, srcTeleW, srcTeleH */
        {4032, 2268, 4032, 2268, 4032, 2268},  /* UHD */
        {2304, 1296, 2784, 1566, 2784, 1566},  /* FHD */
        {3072, 1728, 3696, 2078, 3696, 2078},  /* QHD */
        {1536, 864,  1856, 1044, 1856, 1044},  /* HD */
    };

    bool isSwVdisMode = false;
    int vdisPreviewW, vdisPreviewH;
    if (isSWVdisModeWithParam(targetWidth, targetHeight) == true) {
        isSwVdisMode = true;
        m_getSWVdisPreviewSize(targetWidth, targetHeight, &vdisPreviewW, &vdisPreviewH);
        sizeListLen = (sizeof(dualSolutionVdisSize) / DUAL_SOLUTION_SIZE_MAX) / sizeof(int);
    } else
#endif
    sizeListLen = (sizeof(dualSolutionSize) / DUAL_SOLUTION_SIZE_MAX) / sizeof(int);

    int i;
    for (i = 0; i < sizeListLen; i++) {
#ifdef SUPPORT_SW_VDIS
        if (isSwVdisMode == true) {
            if (dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_DST_W] == vdisPreviewW &&
                    dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_DST_H] == vdisPreviewH) {
                m_dualDstWidth       = dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_DST_W];
                m_dualDstHeight      = dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_DST_H];
                m_dualSrcWideWidth   = dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_SRC_WIDE_W];
                m_dualSrcWideHeight  = dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_SRC_WIDE_H];
                m_dualSrcTeleWidth   = dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_SRC_TELE_W];
                m_dualSrcTeleHeight  = dualSolutionVdisSize[i][DUAL_SOLUTION_SIZE_SRC_TELE_H];
                break;
            }
        } else
#endif
        {
            if (dualSolutionSize[i][DUAL_SOLUTION_SIZE_DST_W] == targetWidth &&
                    dualSolutionSize[i][DUAL_SOLUTION_SIZE_DST_H] == targetHeight) {
                m_dualDstWidth       = dualSolutionSize[i][DUAL_SOLUTION_SIZE_DST_W];
                m_dualDstHeight      = dualSolutionSize[i][DUAL_SOLUTION_SIZE_DST_H];
                m_dualSrcWideWidth   = dualSolutionSize[i][DUAL_SOLUTION_SIZE_SRC_WIDE_W];
                m_dualSrcWideHeight  = dualSolutionSize[i][DUAL_SOLUTION_SIZE_SRC_WIDE_H];
                m_dualSrcTeleWidth   = dualSolutionSize[i][DUAL_SOLUTION_SIZE_SRC_TELE_W];
                m_dualSrcTeleHeight  = dualSolutionSize[i][DUAL_SOLUTION_SIZE_SRC_TELE_H];
                break;
            }
        }
    }

    if (i == sizeListLen)
        return BAD_VALUE;
    else
        return NO_ERROR;
}

void ExynosCamera1Parameters::getDualSolutionSize(int *dstW, int *dstH,
                                                      int *wideW, int *wideH, int *teleW, int *teleH)
{
    *dstW  = m_dualDstWidth;
    *dstH  = m_dualDstHeight;
    *wideW = m_dualSrcWideWidth;
    *wideH = m_dualSrcWideHeight;
    *teleW = m_dualSrcTeleWidth;
    *teleH = m_dualSrcTeleHeight;

    return;
}

void ExynosCamera1Parameters::getZoomList(int **list, int *size)
{
    *list = m_staticInfo->zoomRatioList;
    *size = m_staticInfo->maxZoomLevel;

    return;
}

int ExynosCamera1Parameters::getMasterCam(void)
{
    return m_dualParameters->getMasterCam(m_cameraId);
}

void ExynosCamera1Parameters::setForceWide(bool forceWide)
{
    m_dualParameters->setForceWide(m_cameraId, forceWide);
}

bool ExynosCamera1Parameters::getForceWide(void)
{
    return m_dualParameters->getForceWide(m_cameraId);
}

void ExynosCamera1Parameters::setForceWideCond(bool flagValidFrame, struct camera2_shot_ext *meta)
{
    int32_t lux = (int32_t)meta->shot.udm.ae.vendorSpecific[386];
    int32_t objectDistance = meta->shot.dm.aa.vendor_objectDistanceCm;

    if (flagValidFrame == false)
        return;

    if (lux <= DUAL_LOW_LIGHT_CONDITION_FORCE ||
        (objectDistance  < DUAL_DISTANCE_CONDITION_FORCE && objectDistance > 0)) {
        m_dualParameters->setForceWideCond(m_cameraId, true);
    } else if (lux >= DUAL_LOW_LIGHT_CONDITION_NORMAL &&
               objectDistance >= DUAL_DISTANCE_CONDITION_NORMAL) {
        m_dualParameters->setForceWideCond(m_cameraId, false);
    }
}

bool ExynosCamera1Parameters::getForceWideCond(void)
{
    return m_dualParameters->getForceWideCond(m_cameraId);
}

void ExynosCamera1Parameters::setFusionParam(UniPlugin_DCVOZ_CAMERAPARAM_t *cameraParam)
{
    m_fusionParam = cameraParam;
}

UniPlugin_DCVOZ_CAMERAPARAM_t* ExynosCamera1Parameters::getFusionParam(void)
{
    return m_fusionParam;
}

int ExynosCamera1Parameters::getFusionParamIndex(void)
{
    if (m_fusionParam)
        return m_fusionParam->CameraIndex;
    else
        return UNI_PLUGIN_CAMERA_TYPE_WIDE;
}

int ExynosCamera1Parameters::checkDualAfState(int cameraId, int focusState)
{
    return m_dualParameters->checkDualAfState(cameraId, focusState);
}

void ExynosCamera1Parameters::checkFusionCaptureMode(int cameraId,
                                        struct camera2_shot_ext *wideMeta, struct camera2_shot_ext *teleMeta)
{
    return m_dualParameters->checkFusionCaptureMode(cameraId, wideMeta, teleMeta);
}

bool ExynosCamera1Parameters::getFusionCaptureMode(int cameraId, bool isLogPrint)
{
    return m_dualParameters->getFusionCaptureMode(cameraId, isLogPrint);
}
#endif

#ifdef SAMSUNG_HIFI_LLS
bool ExynosCamera1Parameters::checkHiFiLLSEnable(void)
{
    bool HiFiLLSEnable = false;
    int shotMode = getShotMode();

    if (getSamsungCamera() == true) {
        if (getRecordingHint() == false
#ifdef USE_DUAL_CAMERA
                && !(getDualCameraMode() == false &&
                    getDualMode() == true)
#else
                && getDualMode() == false
#endif
                && (shotMode == SHOT_MODE_AUTO
                    || shotMode == SHOT_MODE_BEAUTY_FACE
                    || shotMode == SHOT_MODE_NIGHT)) {
            HiFiLLSEnable = true;
        }
    }

    CLOGD("[LLS_MBR] HiFiLLSEnable %d (SC %d, RH %d, DM %d, SM %d)",
            HiFiLLSEnable, getSamsungCamera(), getRecordingHint(), getDualMode(), shotMode);

    return HiFiLLSEnable;
}
#endif

status_t ExynosCamera1Parameters::m_adjustPreviewSize(const CameraParameters &params,
                                                     __unused int previewW, __unused int previewH,
                                                     int *newPreviewW, int *newPreviewH,
                                                     int *newCalHwPreviewW, int *newCalHwPreviewH)
{
    int ShotMode = params.getInt("shot-mode");

    /* hack : when app give 1446, we calibrate to 1440 */
    if (*newPreviewW == 1446 && *newPreviewH == 1080) {
        CLOGW("Invalid previewSize(%d/%d). so, calibrate to (1440/%d)", *newPreviewW, *newPreviewH, *newPreviewH);
        *newPreviewW = 1440;
    }

    /* Exynos8890 has MC scaler, so it need not re-calculate size */
#if 0
    if (getRecordingHint() == true && getHighSpeedRecording() == true) {
        int sizeList[SIZE_LUT_INDEX_END];

        if (m_getPreviewSizeList(sizeList) == NO_ERROR) {
            /* On high-speed recording, scaling-up by SCC/SCP occurs the IS-ISP performance degradation.
               The scaling-up might be done by GSC for recording */
            *newPreviewW = (sizeList[BDS_W] < sizeList[TARGET_W])? sizeList[BDS_W] : sizeList[TARGET_W];
            *newPreviewH = (sizeList[BDS_H] < sizeList[TARGET_H])? sizeList[BDS_H] : sizeList[TARGET_H];
        } else {
            CLOGE("m_getPreviewSizeList() fail");
        }
    }

#ifdef USE_DUAL_CAMERA
    if (isFusionEnabled() == true) {
        ExynosRect fusionSrcRect;
        ExynosRect fusionDstRect;

        if (getFusionSize(*newPreviewW, *newPreviewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
            CLOGE("getFusionSize(%d, %d) fail", *newPreviewW, *newPreviewH);

            *newCalHwPreviewW = *newPreviewW;
            *newCalHwPreviewH = *newPreviewH;
        } else {
            *newCalHwPreviewW = fusionSrcRect.w;
            *newCalHwPreviewH = fusionSrcRect.h;
        }
    } else
#endif
    if (getRecordingHint() == true) {
        int videoW = 0, videoH = 0;
        ExynosRect bdsRect;

        getVideoSize(&videoW, &videoH);

        if ((videoW <= *newPreviewW) && (videoH <= *newPreviewH)) {
            {
#if defined(LIMIT_SCP_SIZE_UNTIL_FHD_ON_RECORDING)
                if ((videoW <= 1920 || videoH <= 1080) &&
                    (1920 < *newPreviewW || 1080 < *newPreviewH)) {

                    float videoRatio = ROUND_OFF(((float)videoW / (float)videoH), 2);

                    if (videoRatio == 1.33f) { /* 4:3 */
                        *newCalHwPreviewW = 1440;
                        *newCalHwPreviewH = 1080;
                    } else if (videoRatio == 1.77f) { /* 16:9 */
                        *newCalHwPreviewW = 1920;
                        *newCalHwPreviewH = 1080;
                    } else if (videoRatio == 1.00f) { /* 1:1 */
                        *newCalHwPreviewW = 1088;
                        *newCalHwPreviewH = 1088;
                    } else {
                        *newCalHwPreviewW = *newPreviewW;
                        *newCalHwPreviewH = *newPreviewH;
                    }

                    if (*newCalHwPreviewW != *newPreviewW  ||
                        *newCalHwPreviewH != *newPreviewH) {
                        CLOGW("Limit hw preview size until %d x %d when videoSize(%d x %d)",
                            *newCalHwPreviewW, *newCalHwPreviewH, videoW, videoH);
                    }
                } else
#endif
                {
                    *newCalHwPreviewW = *newPreviewW;
                    *newCalHwPreviewH = *newPreviewH;
                }
            }
        } else {
            /* video size > preview size : Use BDS size for SCP output size */
            {
                CLOGV("preview(%dx%d) is smaller than video(%dx%d)",
                        *newPreviewW, *newPreviewH, videoW, videoH);

                /* If the video ratio is differ with preview ratio,
                   the default ratio is set into preview ratio */
                if (SIZE_RATIO(*newPreviewW, *newPreviewH) != SIZE_RATIO(videoW, videoH))
                    CLOGW(" preview ratio(%dx%d) is not matched with video ratio(%dx%d)",
                            *newPreviewW, *newPreviewH, videoW, videoH);

                if (m_isSupportedPreviewSize(*newPreviewW, *newPreviewH) == false) {
                    CLOGE(" new preview size is invalid(%dx%d)",
                            *newPreviewW, *newPreviewH);
                    return BAD_VALUE;
                }

                /*
                 * This call is to get real preview size.
                 * so, HW dis size must not be added.
                 */
                m_getPreviewBdsSize(&bdsRect);

                *newCalHwPreviewW = bdsRect.w;
                *newCalHwPreviewH = bdsRect.h;
            }
        }
    } else if (getHighResolutionCallbackMode() == true) {
        if(CAMERA_LCD_SIZE == LCD_SIZE_1280_720) {
            *newCalHwPreviewW = 1280;
            *newCalHwPreviewH = 720;
        } else {
            *newCalHwPreviewW = 1920;
            *newCalHwPreviewH = 1080;
        }
    } else {
        *newCalHwPreviewW = *newPreviewW;
        *newCalHwPreviewH = *newPreviewH;
    }
#endif

    /* calibrate H/W aligned size*/
#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true) {
        ExynosRect fusionSrcRect;
        ExynosRect fusionDstRect;
#ifdef SAMSUNG_DUAL_SOLUTION
        if (adjustDualSolutionSize(*newPreviewW, *newPreviewH) != NO_ERROR) {
            CLOGE("[Fusion] unsupported size %d, %d", *newPreviewW, *newPreviewH);
        }
#endif
        if (getFusionSize(*newPreviewW, *newPreviewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
            CLOGE("getFusionSize(%d, %d) fail", *newPreviewW, *newPreviewH);

            *newCalHwPreviewW = *newPreviewW;
            *newCalHwPreviewH = *newPreviewH;
        } else {
            *newCalHwPreviewW = fusionSrcRect.w;
            *newCalHwPreviewH = fusionSrcRect.h;
        }
    } else
#endif
    if (ShotMode == SHOT_MODE_PANORAMA) {
        if(SIZE_RATIO(*newPreviewW, *newPreviewH) == SIZE_RATIO(4, 3)) {
            *newCalHwPreviewW = PANORAMA_PREVIEW_WIDTH_RATIO_4_3;
            *newCalHwPreviewH = PANORAMA_PREVIEW_HEIGHT_RATIO_4_3;
        } else {
            *newCalHwPreviewW = PANORAMA_PREVIEW_WIDTH_RATIO_16_9;
            *newCalHwPreviewH = PANORAMA_PREVIEW_HEIGHT_RATIO_16_9;
        }
    } else {
        *newCalHwPreviewW = *newPreviewW;
        *newCalHwPreviewH = *newPreviewH;
    }

#ifdef SUPPORT_SW_VDIS
    if (isSWVdisModeWithParam(*newPreviewW, *newPreviewH) == true && getDualCameraMode() == false) {
        m_getSWVdisPreviewSize(*newPreviewW, *newPreviewH,
                                newCalHwPreviewW, newCalHwPreviewH);
    }
#endif /*SUPPORT_SW_VDIS*/

#ifdef SAMSUNG_HYPER_MOTION
    m_isHyperMotionMode(params);
    if (getHyperMotionMode() == true) {
        m_getHyperMotionPreviewSize(*newPreviewW, *newPreviewH,
                                    newCalHwPreviewW, newCalHwPreviewH);
    }
#endif /*SAMSUNG_HYPER_MOTION*/

    return NO_ERROR;
}

/* TODO: Who explane this offset value? */
/* F/W's middle value is 5, and step is -4, -3, -2, -1, 0, 1, 2, 3, 4 */
void ExynosCamera1Parameters::m_setExposureCompensation(int32_t value)
{
    setMetaCtlExposureCompensation(&m_metadata, value);
#ifdef USE_SUBDIVIDED_EV
    setMetaCtlExposureCompensationStep(&m_metadata, m_staticInfo->exposureCompensationStep);
#endif
}

int ExynosCamera1Parameters::getBayerFormat(int pipeId)
{
    int bayerFormat = V4L2_PIX_FMT_SBGGR16;

    switch (pipeId) {
    case PIPE_FLITE:
    case PIPE_VC0:
    case PIPE_3AA:
    case PIPE_FLITE_REPROCESSING:
    case PIPE_3AA_REPROCESSING:
        bayerFormat = CAMERA_FLITE_BAYER_FORMAT;
        break;
    case PIPE_3AC:
        bayerFormat = CAMERA_3AC_BAYER_FORMAT;
        break;
    case PIPE_3AP:
    case PIPE_ISP:
#ifdef USE_DUAL_CAMERA
        if (m_cameraId == CAMERA_ID_BACK_1) {
            bayerFormat = V4L2_PIX_FMT_SBGGR12;
        } else {
            bayerFormat = CAMERA_3AP_BAYER_FORMAT;
        }
#else
        bayerFormat = CAMERA_3AP_BAYER_FORMAT;
#endif
        break;
    case PIPE_3AC_REPROCESSING:
    case PIPE_3AP_REPROCESSING:
    case PIPE_ISP_REPROCESSING:
        bayerFormat = CAMERA_3AP_REPROCESSING_BAYER_FORMAT;
        break;
    default:
        CLOGW("Invalid pipeId(%d)", pipeId);
        break;
    }

    return bayerFormat;
}

int32_t ExynosCamera1Parameters::getExposureCompensation(void)
{
    int32_t expCompensation;
    getMetaCtlExposureCompensation(&m_metadata, &expCompensation);

    return expCompensation;
}

void ExynosCamera1Parameters::m_setMeteringAreas(uint32_t num, ExynosRect2 *rect2s, int *weights)
{
    uint32_t maxNumMeteringAreas = getMaxNumMeteringAreas();

    if(getSamsungCamera()) {
        maxNumMeteringAreas = 1;
    }

    if (maxNumMeteringAreas == 0) {
        CLOGV("maxNumMeteringAreas is 0. so, ignored");
        return;
    }

    if (maxNumMeteringAreas < num)
        num = maxNumMeteringAreas;

    if (getAutoExposureLock() == true) {
        CLOGD("autoExposure is Locked");
        return;
    }

    if (num == 1) {
#ifdef CAMERA_GED_FEATURE
        int meteringMode = getMeteringMode();

        if (isRectNull(&rect2s[0]) == true) {
            switch (meteringMode) {
                case METERING_MODE_SPOT:
                    /*
                     * Even if SPOT metering mode, area must set valid values,
                     * but areas was invalid values, we change mode to CENTER.
                     */
                    m_setMeteringMode(METERING_MODE_CENTER);
                    m_cameraInfo.isTouchMetering = false;
                    break;
                case METERING_MODE_AVERAGE:
                case METERING_MODE_CENTER:
                case METERING_MODE_MATRIX:
                default:
                    /* adjust metering setting */
                    break;
            }
        } else {
#ifdef USE_MULTI_FACING_SINGLE_CAMERA
            if (getCameraDirection() == 1) { /* CAMERA_FACING_DIRECTION_FRONT */
                int tempInt;
                for (uint32_t k = 0; k < num; k++) {
                    tempInt = rect2s[k].y1;
                    rect2s[k].y1 = -rect2s[k].y2;
                    rect2s[k].y2 = -tempInt;
               }
            }
#endif
            switch (meteringMode) {
                case METERING_MODE_CENTER:
                    /*
                     * SPOT metering mode in GED camera App was not set METERING_MODE_SPOT,
                     * but set metering areas only.
                     */
                    m_setMeteringMode(METERING_MODE_SPOT);
                    m_cameraInfo.isTouchMetering = true;
                    break;
                case METERING_MODE_AVERAGE:
                case METERING_MODE_MATRIX:
                case METERING_MODE_SPOT:
                default:
                    /* adjust metering setting */
                    break;
            }
        }
#endif
    } else {
        if (num > 1 && isRectEqual(&rect2s[0], &rect2s[1]) == false) {
            /* if MATRIX mode support, mode set METERING_MODE_MATRIX */
            m_setMeteringMode(METERING_MODE_AVERAGE);
            m_cameraInfo.isTouchMetering = false;
        } else {
            m_setMeteringMode(METERING_MODE_AVERAGE);
            m_cameraInfo.isTouchMetering = false;
        }
    }

    ExynosRect bayerCropRegionRect;
    ExynosRect cropRegionRect;
    ExynosRect2 newRect2;

    getHwBayerCropRegion(&bayerCropRegionRect.w, &bayerCropRegionRect.h, &bayerCropRegionRect.x, &bayerCropRegionRect.y);

    if (isUseIspInputCrop() == true
        || isUseMcscInputCrop() == true) {
        status_t ret = NO_ERROR;
        int zoomLevel = 0;
        float zoomRatio = getZoomRatio(0) / 1000;

        zoomLevel = getZoomLevel();
        zoomRatio = getZoomRatio(zoomLevel) / 1000;
        ret = getCropRectAlign(bayerCropRegionRect.w, bayerCropRegionRect.h,
                bayerCropRegionRect.w, bayerCropRegionRect.h,
                &cropRegionRect.x, &cropRegionRect.y,
                &cropRegionRect.w, &cropRegionRect.h,
                2, 2,
                zoomLevel, zoomRatio);
    } else {
        cropRegionRect = bayerCropRegionRect;
    }

    for (uint32_t i = 0; i < num; i++) {
        bool isChangeMeteringArea = false;
#ifdef CAMERA_GED_FEATURE
        if (isRectNull(&rect2s[i]) == false)
            isChangeMeteringArea = true;
        else
            isChangeMeteringArea = false;
#else
        if ((isRectNull(&rect2s[i]) == false) ||((isRectNull(&rect2s[i]) == true) && (getMeteringMode() == METERING_MODE_SPOT)))
            isChangeMeteringArea = true;
#ifdef TOUCH_AE
        else if((getMeteringMode() == METERING_MODE_SPOT_TOUCH) || (getMeteringMode() == METERING_MODE_MATRIX_TOUCH)
            || (getMeteringMode() == METERING_MODE_CENTER_TOUCH) || (getMeteringMode() == METERING_MODE_AVERAGE_TOUCH))
            isChangeMeteringArea = true;
#endif
        else
            isChangeMeteringArea = false;
#endif
        if (isChangeMeteringArea == true) {
            CLOGD("from Service (%d %d %d %d) %d", rect2s->x1, rect2s->y1, rect2s->x2, rect2s->y2, getMeteringMode());
            newRect2 = convertingAndroidArea2HWAreaBcropOut(&rect2s[i], &cropRegionRect);
            CLOGD("to FW (%d %d %d %d) %d", newRect2.x1, newRect2.y1, newRect2.x2, newRect2.y2, weights[i]);
            setMetaCtlAeRegion(&m_metadata, newRect2.x1, newRect2.y1, newRect2.x2, newRect2.y2, weights[i]);
        }
    }
}

status_t ExynosCamera1Parameters::checkPictureFormat(const CameraParameters& params)
{
    int curPictureFormat = 0;
    int newPictureFormat = 0;
    int newHwPictureFormat = 0;
    const char *strNewPictureFormat = params.getPictureFormat();
    const char *strCurPictureFormat = m_params.getPictureFormat();
#ifdef SAMSUNG_DNG
    int shotMode = params.getInt("shot-mode");
#endif

    if (strNewPictureFormat == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newPictureFormat %s", strNewPictureFormat);

    if (!strcmp(strNewPictureFormat, CameraParameters::PIXEL_FORMAT_JPEG)) {
#ifdef SAMSUNG_DNG
        setDNGCaptureModeOn(false);
#endif
        newPictureFormat = V4L2_PIX_FMT_JPEG;
        newHwPictureFormat = SCC_OUTPUT_COLOR_FMT;
    }
#ifdef SAMSUNG_DNG
    else if (!strcmp(strNewPictureFormat, "raw+jpeg")) {
        newPictureFormat = V4L2_PIX_FMT_JPEG;
        newHwPictureFormat = SCC_OUTPUT_COLOR_FMT;

        if (shotMode == SHOT_MODE_PRO_MODE) {
            setDNGCaptureModeOn(true);
        } else {
            setDNGCaptureModeOn(false);
            CLOGW("Picture format(%s) is not supported!, shot mode(%d)",
                     strNewPictureFormat, shotMode);
        }
    }
#endif
    else if (!strcmp(strNewPictureFormat, CameraParameters::PIXEL_FORMAT_YUV420SP)
#ifdef SAMSUNG_TN_FEATURE
            || !strcmp(strNewPictureFormat, CameraParameters::PIXEL_FORMAT_YUV420SP_NV21)
#endif
    ) {
#ifdef SAMSUNG_DNG
        setDNGCaptureModeOn(false);
#endif
        newPictureFormat = V4L2_PIX_FMT_NV21;
        newHwPictureFormat = SCC_OUTPUT_COLOR_FMT;
    } else {
#ifdef SAMSUNG_DNG
        setDNGCaptureModeOn(false);
#endif
        CLOGE(" Picture format(%s) is not supported!", strNewPictureFormat);
        return BAD_VALUE;
    }

    curPictureFormat = getPictureFormat();

    if (newPictureFormat != curPictureFormat) {
        CLOGI("[Parameters]Picture format changed, cur(%s) -> new(%s)",
                strCurPictureFormat, strNewPictureFormat);
        m_setPictureFormat(newPictureFormat);
        m_setHwPictureFormat(newHwPictureFormat);
        m_params.setPictureFormat(strNewPictureFormat);
    }

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::checkMeteringMode(const CameraParameters& params)
{
    const char *strNewMeteringMode = params.get("metering");
    int newMeteringMode = -1;
    int curMeteringMode = -1;
#ifdef SAMSUNG_COMPANION
    const char *strNewRTHdr = params.get("rt-hdr");
#endif
    if (strNewMeteringMode == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewMeteringMode %s", strNewMeteringMode);

#ifdef SAMSUNG_COMPANION
    if ((strNewRTHdr != NULL && !strcmp(strNewRTHdr, "on"))
#ifdef TOUCH_AE
        && strcmp(strNewMeteringMode, "weighted-matrix")
#endif
        ) {
        newMeteringMode = METERING_MODE_MATRIX;
        CLOGD("[setParameters]newMeteringMode %d in RT-HDR", newMeteringMode);
    } else
#endif
    {
        if (!strcmp(strNewMeteringMode, "average"))
            newMeteringMode = METERING_MODE_AVERAGE;
        else if (!strcmp(strNewMeteringMode, "center"))
            newMeteringMode = METERING_MODE_CENTER;
        else if (!strcmp(strNewMeteringMode, "matrix"))
            newMeteringMode = METERING_MODE_MATRIX;
        else if (!strcmp(strNewMeteringMode, "spot"))
            newMeteringMode = METERING_MODE_SPOT;
#ifdef TOUCH_AE
        else if (!strcmp(strNewMeteringMode, "weighted-center"))
            newMeteringMode = METERING_MODE_CENTER_TOUCH;
        else if (!strcmp(strNewMeteringMode, "weighted-matrix"))
            newMeteringMode = METERING_MODE_MATRIX_TOUCH;
        else if (!strcmp(strNewMeteringMode, "weighted-spot"))
            newMeteringMode = METERING_MODE_SPOT_TOUCH;
        else if (!strcmp(strNewMeteringMode, "weighted-average"))
            newMeteringMode = METERING_MODE_AVERAGE_TOUCH;
#endif
        else {
            CLOGE("Invalid metering newMetering(%s)", strNewMeteringMode);
            return UNKNOWN_ERROR;
        }
    }

    curMeteringMode = getMeteringMode();

    m_setMeteringMode(newMeteringMode);
#if defined(SAMSUNG_COMPANION) && !defined(CAMERA_GED_FEATURE)
    if ((strNewRTHdr != NULL && strcmp(strNewRTHdr, "on"))
#ifdef TOUCH_AE
        || !strcmp(strNewMeteringMode, "weighted-matrix")
#endif
        ) {
        m_params.set("metering", strNewMeteringMode);
    }
#else
    m_params.set("metering", strNewMeteringMode);
#endif

    if (curMeteringMode != newMeteringMode) {
        CLOGI(" Metering Area is changed (%d -> %d)", curMeteringMode, newMeteringMode);
        m_flagMeteringRegionChanged = true;
    }

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::checkSceneMode(const CameraParameters& params)
{
    int  newSceneMode = -1;
    int  curSceneMode = -1;
    const char *strNewSceneMode = params.get(CameraParameters::KEY_SCENE_MODE);

    if (strNewSceneMode == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewSceneMode %s", strNewSceneMode);

    if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_AUTO)) {
        newSceneMode = SCENE_MODE_AUTO;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_ACTION)) {
        newSceneMode = SCENE_MODE_ACTION;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_PORTRAIT)) {
        newSceneMode = SCENE_MODE_PORTRAIT;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_LANDSCAPE)) {
        newSceneMode = SCENE_MODE_LANDSCAPE;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_NIGHT)) {
        newSceneMode = SCENE_MODE_NIGHT;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_NIGHT_PORTRAIT)) {
        newSceneMode = SCENE_MODE_NIGHT_PORTRAIT;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_THEATRE)) {
        newSceneMode = SCENE_MODE_THEATRE;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_BEACH)) {
        newSceneMode = SCENE_MODE_BEACH;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_SNOW)) {
        newSceneMode = SCENE_MODE_SNOW;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_SUNSET)) {
        newSceneMode = SCENE_MODE_SUNSET;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_STEADYPHOTO)) {
        newSceneMode = SCENE_MODE_STEADYPHOTO;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_FIREWORKS)) {
        newSceneMode = SCENE_MODE_FIREWORKS;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_SPORTS)) {
        newSceneMode = SCENE_MODE_SPORTS;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_PARTY)) {
        newSceneMode = SCENE_MODE_PARTY;
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_CANDLELIGHT)) {
        newSceneMode = SCENE_MODE_CANDLELIGHT;
#ifdef SAMSUNG_COMPANION
    } else if (!strcmp(strNewSceneMode, CameraParameters::SCENE_MODE_HDR)) {
        newSceneMode = SCENE_MODE_HDR;
#endif
#ifdef SAMSUNG_FOOD_MODE
    } else if (!strcmp(strNewSceneMode, "food")) {
        enum aa_aemode aeMode = AA_AEMODE_MATRIX;
        newSceneMode = SCENE_MODE_FOOD;

        m_setExposureCompensation(2);
        setMetaCtlAeMode(&m_metadata, aeMode);
#endif
    } else {
        CLOGE("[Parameters]unmatched scene_mode(%s)", strNewSceneMode);
        return BAD_VALUE;
    }

    curSceneMode = getSceneMode();

    if (curSceneMode != newSceneMode) {
        m_setSceneMode(newSceneMode);
        m_params.set(CameraParameters::KEY_SCENE_MODE, strNewSceneMode);
#ifdef SAMSUNG_COMPANION
        if(getUseCompanion() == true) {
            if (newSceneMode == SCENE_MODE_HDR)
                checkSceneRTHdr(true);
            else if (curSceneMode == SCENE_MODE_HDR)
                checkSceneRTHdr(false);
        }

        if ((newSceneMode == SCENE_MODE_HDR) || (curSceneMode == SCENE_MODE_HDR))
            m_setRestartPreview(true);
#endif
        updatePreviewFpsRange();
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setSceneMode(int value)
{
    enum aa_mode mode = AA_CONTROL_AUTO;
    enum aa_scene_mode sceneMode = AA_SCENE_MODE_FACE_PRIORITY;

    switch (value) {
    case SCENE_MODE_PORTRAIT:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_PORTRAIT;
        break;
    case SCENE_MODE_LANDSCAPE:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_LANDSCAPE;
        break;
    case SCENE_MODE_NIGHT:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_NIGHT;
        break;
    case SCENE_MODE_BEACH:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_BEACH;
        break;
    case SCENE_MODE_SNOW:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_SNOW;
        break;
    case SCENE_MODE_SUNSET:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_SUNSET;
        break;
    case SCENE_MODE_FIREWORKS:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_FIREWORKS;
        break;
    case SCENE_MODE_SPORTS:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_SPORTS;
        break;
    case SCENE_MODE_PARTY:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_PARTY;
        break;
    case SCENE_MODE_CANDLELIGHT:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_CANDLELIGHT;
        break;
    case SCENE_MODE_STEADYPHOTO:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_STEADYPHOTO;
        break;
    case SCENE_MODE_ACTION:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_ACTION;
        break;
    case SCENE_MODE_NIGHT_PORTRAIT:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_NIGHT_PORTRAIT;
        break;
    case SCENE_MODE_THEATRE:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_THEATRE;
        break;
#ifdef SAMSUNG_FOOD_MODE
    case SCENE_MODE_FOOD:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_FOOD;
        break;
#endif
    case SCENE_MODE_AUTO:
    default:
        mode = AA_CONTROL_AUTO;
        sceneMode = AA_SCENE_MODE_FACE_PRIORITY;
        break;
    }

    m_cameraInfo.sceneMode = value;
    setMetaCtlSceneMode(&m_metadata, mode, sceneMode);
    m_cameraInfo.whiteBalanceMode = m_convertMetaCtlAwbMode(&m_metadata);
}

status_t ExynosCamera1Parameters::checkFocusMode(const CameraParameters& params)
{
    int  newFocusMode = -1;
    const char *strFocusMode = params.get(CameraParameters::KEY_FOCUS_MODE);
    const char *strNewFocusMode = m_adjustFocusMode(strFocusMode);

    if (strNewFocusMode == NULL || m_scenario == SCENARIO_SECURE) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewFocusMode %s", strNewFocusMode);

    if (!strcmp(strNewFocusMode, CameraParameters::FOCUS_MODE_AUTO)) {
        newFocusMode = FOCUS_MODE_AUTO;
        m_params.set(CameraParameters::KEY_FOCUS_DISTANCES,
                BACK_CAMERA_AUTO_FOCUS_DISTANCES_STR);
    } else if (!strcmp(strNewFocusMode, CameraParameters::FOCUS_MODE_INFINITY)) {
        newFocusMode = FOCUS_MODE_INFINITY;
        m_params.set(CameraParameters::KEY_FOCUS_DISTANCES,
                BACK_CAMERA_INFINITY_FOCUS_DISTANCES_STR);
    } else if (!strcmp(strNewFocusMode, CameraParameters::FOCUS_MODE_MACRO)) {
        newFocusMode = FOCUS_MODE_MACRO;
        m_params.set(CameraParameters::KEY_FOCUS_DISTANCES,
                BACK_CAMERA_MACRO_FOCUS_DISTANCES_STR);
    } else if (!strcmp(strNewFocusMode, CameraParameters::FOCUS_MODE_FIXED)) {
        newFocusMode = FOCUS_MODE_FIXED;
    } else if (!strcmp(strNewFocusMode, CameraParameters::FOCUS_MODE_EDOF)) {
        newFocusMode = FOCUS_MODE_EDOF;
    } else if (!strcmp(strNewFocusMode, CameraParameters::FOCUS_MODE_CONTINUOUS_VIDEO)) {
        newFocusMode = FOCUS_MODE_CONTINUOUS_VIDEO;
    } else if (!strcmp(strNewFocusMode, CameraParameters::FOCUS_MODE_CONTINUOUS_PICTURE)) {
        newFocusMode = FOCUS_MODE_CONTINUOUS_PICTURE;
    } else if (!strcmp(strNewFocusMode, "face-priority")) {
        newFocusMode = FOCUS_MODE_CONTINUOUS_PICTURE;
    } else if (!strcmp(strNewFocusMode, "continuous-picture-macro")) {
        newFocusMode = FOCUS_MODE_CONTINUOUS_PICTURE_MACRO;
#ifdef SAMSUNG_OT
    } else if (!strcmp(strNewFocusMode, "object-tracking-picture")) {
        newFocusMode = FOCUS_MODE_OBJECT_TRACKING_PICTURE;
    } else if (!strcmp(strNewFocusMode, "object-tracking-video")) {
        newFocusMode = FOCUS_MODE_OBJECT_TRACKING_VIDEO;
#endif
#ifdef SAMSUNG_MANUAL_FOCUS
    } else if (!strcmp(strNewFocusMode, "manual")) {
        newFocusMode = FOCUS_MODE_MANUAL;
#endif
#ifdef SAMSUNG_FIXED_FACE_FOCUS
    } else if (!strcmp(strNewFocusMode, "fixed-face")) {
        newFocusMode = FOCUS_MODE_FIXED_FACE;
#endif
    } else {
        CLOGE("unmatched focus_mode(%s)", strNewFocusMode);
        return BAD_VALUE;
    }

    if (!(newFocusMode & getSupportedFocusModes())){
        CLOGE(" Focus mode(%s) is not supported!", strNewFocusMode);
        return BAD_VALUE;
    }

#ifdef SAMSUNG_MANUAL_FOCUS
    /* Set focus distance to -1 if focus mode is changed from MANUAL to others */
    if ((FOCUS_MODE_MANUAL == getFocusMode()) && (FOCUS_MODE_MANUAL != newFocusMode)) {
        m_setFocusDistance(-1);
    }
#endif

    m_setFocusMode(newFocusMode);
    m_params.set(CameraParameters::KEY_FOCUS_MODE, strNewFocusMode);

    return NO_ERROR;
}

const char *ExynosCamera1Parameters::m_adjustFocusMode(const char *focusMode)
{
    const char *newFocusMode = NULL;

    /* TODO: vendor specific adjust */

    newFocusMode = focusMode;

    return newFocusMode;
}

void ExynosCamera1Parameters::m_setFocusMode(int focusMode)
{
    m_cameraInfo.focusMode = focusMode;

    if(getZoomActiveOn()) {
        CLOGD("[setParameters]zoom moving..");
        return;
    }

    /* TODO: Notify auto focus activity */
    if(getPreviewRunning() == true
#ifdef SAMSUNG_FIXED_FACE_FOCUS
        || focusMode == FOCUS_MODE_FIXED_FACE
#endif
    ) {
        CLOGD("set Focus Mode!!!!");
        m_activityControl->setAutoFocusMode(focusMode);
    } else {
        m_setFocusmodeSetting = true;
    }

#ifdef SAMSUNG_OT
    if(m_cameraInfo.focusMode == FOCUS_MODE_OBJECT_TRACKING_VIDEO
       || m_cameraInfo.focusMode == FOCUS_MODE_OBJECT_TRACKING_PICTURE) {
        m_startObjectTracking = true;
    }
    else
        m_startObjectTracking = false;
#endif
}

#ifdef SAMSUNG_MANUAL_FOCUS
status_t ExynosCamera1Parameters::checkFocusDistance(const CameraParameters& params)
{
    int newFocusDistance = params.getInt("focus-distance");
    int curFocusDistance = -1;

    if (newFocusDistance < 0) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newFocusDistance %d", newFocusDistance);

    curFocusDistance = getFocusDistance();

    if (curFocusDistance != newFocusDistance) {
        if (FOCUS_MODE_MANUAL != getFocusMode()) {
            newFocusDistance = -1;
        }

        m_setFocusDistance(newFocusDistance);
        m_params.set("focus-distance", newFocusDistance);
    }

    return NO_ERROR;
}

/**
 * m_setFocusDistance: set focus distance.
 *
 * distance should be -1, 0 or value greater than 0.
 */
void ExynosCamera1Parameters::m_setFocusDistance(int32_t distance)
{
    float metaDistance;

    if (distance < -1) {
        CLOGE("Invalid new focus distance (%d)", distance);
        return;
    }

    if (!distance) {
        metaDistance = 0;
    } else if (-1 == distance) {
        metaDistance = -1;
    } else {
        /* distance variable is in milimeters.
         * And if distance variable is converted into meter unit(called as A),
         * meta distance value is as followings:
         *     metaDistance = 1 / A     */
        metaDistance = 1000 / (float)distance;
    }

    setMetaCtlFocusDistance(&m_metadata, metaDistance);
}

int32_t ExynosCamera1Parameters::getFocusDistance(void)
{
    float metaDistance = 0.0f;

    getMetaCtlFocusDistance(&m_metadata, &metaDistance);

    /* Focus distance 0 means infinite */
    if (0.0f == metaDistance) {
        return -2;
    } else if (-1.0f == metaDistance) {
        return -1;
    }

    return (int32_t)(1000 / metaDistance);
}
#endif /* SAMSUNG_MANUAL_FOCUS */

status_t ExynosCamera1Parameters::checkFocusAreas(const CameraParameters& params)
{
    int ret = NO_ERROR;
    const char *newFocusAreas = params.get(CameraParameters::KEY_FOCUS_AREAS);
    int newNumFocusAreas = params.getInt(CameraParameters::KEY_MAX_NUM_FOCUS_AREAS);
    int curFocusMode = getFocusMode();
    uint32_t maxNumFocusAreas = getMaxNumFocusAreas();
    int focusAreasCount = 0;

    if (newFocusAreas == NULL) {
        ExynosRect2 nullRect2[1];
        nullRect2[0].x1 = 0;
        nullRect2[0].y1 = 0;
        nullRect2[0].x2 = 0;
        nullRect2[0].y2 = 0;

        if (m_cameraInfo.numValidFocusArea != 0)
            m_setFocusAreas(0, nullRect2, NULL);

        return NO_ERROR;
    }

    if (newNumFocusAreas <= 0)
        focusAreasCount = maxNumFocusAreas;
    else
        focusAreasCount = newNumFocusAreas;

    CLOGD("[setParameters]newFocusAreas %s, numFocusAreas(%d)", newFocusAreas, focusAreasCount);

    /* In CameraParameters.h */
    /*
     * Focus area only has effect if the cur focus mode is FOCUS_MODE_AUTO,
     * FOCUS_MODE_MACRO, FOCUS_MODE_CONTINUOUS_VIDEO, or
     * FOCUS_MODE_CONTINUOUS_PICTURE.
     */
    if (curFocusMode & FOCUS_MODE_AUTO
         || curFocusMode & FOCUS_MODE_MACRO
         || curFocusMode & FOCUS_MODE_CONTINUOUS_VIDEO
         || curFocusMode & FOCUS_MODE_CONTINUOUS_PICTURE
         || curFocusMode & FOCUS_MODE_CONTINUOUS_PICTURE_MACRO
#ifdef SAMSUNG_OT
         || curFocusMode & FOCUS_MODE_OBJECT_TRACKING_VIDEO
         || curFocusMode & FOCUS_MODE_OBJECT_TRACKING_PICTURE
#endif
    ) {

        /* ex : (-10,-10,0,0,300),(0,0,10,10,700) */
        ExynosRect2 *rect2s = new ExynosRect2[focusAreasCount];
        int         *weights = new int[focusAreasCount];

        uint32_t validFocusedAreas = bracketsStr2Ints((char *)newFocusAreas, focusAreasCount, rect2s, weights, 1);

        /* Check duplicate area */
        if (validFocusedAreas > 1) {
            for (uint32_t k = 0; k < validFocusedAreas; k++) {
                if (rect2s[k].x1 == rect2s[k+1].x1 &&
                        rect2s[k].y1 == rect2s[k+1].y1 &&
                        rect2s[k].x2 == rect2s[k+1].x2 &&
                        rect2s[k].y2 == rect2s[k+1].y2)
                    validFocusedAreas = 0;
            }
        }

        if (0 < validFocusedAreas) {
            /* CameraParameters.h */
            /*
              * A special case of single focus area (0,0,0,0,0) means driver to decide
              * the focus area. For example, the driver may use more signals to decide
              * focus areas and change them dynamically. Apps can set (0,0,0,0,0) if they
              * want the driver to decide focus areas.
              */
            m_setFocusAreas(validFocusedAreas, rect2s, weights);
            m_params.set(CameraParameters::KEY_FOCUS_AREAS, newFocusAreas);
        } else {
            CLOGE("FocusAreas value is invalid");
            ret = UNKNOWN_ERROR;
        }

        delete [] rect2s;
        delete [] weights;
    }

    return ret;
}

void ExynosCamera1Parameters::m_setFocusAreas(uint32_t numValid, ExynosRect2 *rect2s, int *weights)
{
#ifdef SAMSUNG_OT
    int curFocusMode = getFocusMode();
#endif
    uint32_t maxNumFocusAreas = getMaxNumFocusAreas();
    if (maxNumFocusAreas < numValid)
        numValid = maxNumFocusAreas;

    ExynosRect2 newRect2(0,0,0,0);
    int defaultWeight = 1000;

#ifdef USE_CP_FUSION_LIB
    if (1) {
        /* m_setFocusMode(FOCUS_MODE_AUTO); */

        ExynosRect cropRegionRect;

        // This ROI is set by according to F/W guideLine.
        int androidFocusRoiW = 2000 / 10;
        int androidFocusRoiH = 2000 / 5;

        // we put ROI in center. so, divide by 2.
        newRect2.x1 = -androidFocusRoiW / 2;
        newRect2.y1 = -androidFocusRoiH / 2;
        newRect2.x2 =  androidFocusRoiW / 2;
        newRect2.y2 =  androidFocusRoiH / 2;

        getHwBayerCropRegion(&cropRegionRect.w, &cropRegionRect.h, &cropRegionRect.x, &cropRegionRect.y);

        newRect2 = convertingAndroidArea2HWAreaBcropOut(&newRect2, &cropRegionRect);
    }
#else //USE_CP_FUSION_LIB

    if ((numValid == 1 || numValid == 0) && (isRectNull(&rect2s[0]) == true)) {
        /* m_setFocusMode(FOCUS_MODE_AUTO); */

        m_activityControl->setAutoFcousArea(newRect2, defaultWeight);

        m_activityControl->touchAFMode = false;
        m_activityControl->touchAFModeForFlash = false;
    }
#endif //USE_CP_FUSION_LIB
    else {
#ifdef USE_MULTI_FACING_SINGLE_CAMERA
        if (getCameraDirection() == 1) { /* CAMERA_FACING_DIRECTION_FRONT */
            int tempInt;
            for (uint32_t k = 0; k < numValid; k++) {
                tempInt = rect2s[k].y1;
                rect2s[k].y1 = -rect2s[k].y2;
                rect2s[k].y2 = -tempInt;
            }
        }
#endif
#ifdef SAMSUNG_OT
        if(curFocusMode & FOCUS_MODE_OBJECT_TRACKING_VIDEO
            || curFocusMode & FOCUS_MODE_OBJECT_TRACKING_PICTURE
            || m_objectTrackingGet == true) {
            for (uint32_t i = 0; i < numValid; i++) {
                m_objectTrackingArea[i] = rect2s[i];
                m_objectTrackingWeight[i] = weights[i];
                m_objectTrackingAreaChanged = true;

                m_activityControl->touchAFMode = false;
                m_activityControl->touchAFModeForFlash = false;
            }

            m_cameraInfo.numValidFocusArea = numValid;

            return;
        }
#endif
        ExynosRect bayerCropRegionRect;
        ExynosRect cropRegionRect;

        getHwBayerCropRegion(&bayerCropRegionRect.w, &bayerCropRegionRect.h, &bayerCropRegionRect.x, &bayerCropRegionRect.y);

        if (isUseIspInputCrop() == true
            || isUseMcscInputCrop() == true) {
            status_t ret = NO_ERROR;
            int zoomLevel = 0;
            float zoomRatio = getZoomRatio(0) / 1000;

            zoomLevel = getZoomLevel();
            zoomRatio = getZoomRatio(zoomLevel) / 1000;
            ret = getCropRectAlign(bayerCropRegionRect.w, bayerCropRegionRect.h,
                    bayerCropRegionRect.w, bayerCropRegionRect.h,
                    &cropRegionRect.x, &cropRegionRect.y,
                    &cropRegionRect.w, &cropRegionRect.h,
                    2, 2,
                    zoomLevel, zoomRatio);
        } else {
            cropRegionRect = bayerCropRegionRect;
        }

        for (uint32_t i = 0; i < numValid; i++) {
#ifdef SAMSUNG_DUAL_SOLUTION
            if (getDualCameraMode() == true && getFusionParam()) {
                int masterCameraId = -1;
                int slaveCameraId = -1;
                int index = 0;
                bool doShift = false;

                getDualCameraId(&masterCameraId, &slaveCameraId);
                index = getFusionParamIndex();

                switch(getDualCameraSyncType()) {
                    case SYNC_TYPE_SYNC:
                        if (m_cameraId == masterCameraId) {
                            if (index == UNI_PLUGIN_CAMERA_TYPE_WIDE && m_needPreviewMargin) {
                                doShift = true;
                            }
                        }
                        if (m_cameraId == slaveCameraId) {
                            if (index == UNI_PLUGIN_CAMERA_TYPE_TELE && m_needPreviewMargin) {
                                doShift = true;
                            }
                        }
                        break;
                    case SYNC_TYPE_SWITCH:
                        if (m_cameraId == slaveCameraId) {
                            if (m_needPreviewMargin) {
                                doShift = true;
                            }
                        }
                        break;
                    default:
                        break;
                }

                if (doShift) {
                    UniPlugin_DCVOZ_CAMERAPARAM_t *fusionParam;
                    fusionParam = getFusionParam();

                    rect2s[i].x1 -= fusionParam->PreviewImageShiftX;
                    rect2s[i].y1 -= fusionParam->PreviewImageShiftY;
                    rect2s[i].x2 -= fusionParam->PreviewImageShiftX;
                    rect2s[i].y2 -= fusionParam->PreviewImageShiftY;
                }
            }
#endif
            newRect2 = convertingAndroidArea2HWAreaBcropOut(&rect2s[i], &cropRegionRect);
            /*setMetaCtlAfRegion(&m_metadata, rect2s[i].x1, rect2s[i].y1,
                                    rect2s[i].x2, rect2s[i].y2, weights[i]);*/
            m_activityControl->setAutoFcousArea(newRect2, weights[i]);

            defaultWeight = weights[i];
        }
        m_activityControl->touchAFMode = true;
        m_activityControl->touchAFModeForFlash = true;
    }

    // f/w support only one region now.
    setMetaCtlAfRegion(&m_metadata, newRect2.x1, newRect2.y1, newRect2.x2, newRect2.y2, defaultWeight);

    m_cameraInfo.numValidFocusArea = numValid;
}

debug_attribute_t *ExynosCamera1Parameters::getDebug2Attribute(void)
{
    return &mDebugInfo2;
}

void ExynosCamera1Parameters::m_setExifChangedAttribute(exif_attribute_t    *exifInfo,
                                                        ExynosRect          *pictureRect,
                                                        ExynosRect          *thumbnailRect,
                                                        __unused camera2_dm *dm,
                                                        camera2_udm         *udm,
#ifdef SAMSUNG_DUAL_CAPTURE_SOLUTION
                                                        __unused camera2_dm *dm2,
                                                        camera2_udm         *udm2,
#endif
                                                        bool                useDebugInfo2)
{
    debug_attribute_t &debugInfo = ((useDebugInfo2 == false) ? mDebugInfo : mDebugInfo2);

    /* 2 0th IFD TIFF Tags */
    /* 3 Width */
    exifInfo->width = pictureRect->w;
    /* 3 Height */
    exifInfo->height = pictureRect->h;

    /* 3 Orientation */
    switch (m_cameraInfo.rotation) {
    case 90:
        exifInfo->orientation = EXIF_ORIENTATION_90;
        break;
    case 180:
        exifInfo->orientation = EXIF_ORIENTATION_180;
        break;
    case 270:
        exifInfo->orientation = EXIF_ORIENTATION_270;
        break;
    case 0:
    default:
        exifInfo->orientation = EXIF_ORIENTATION_UP;
        break;
    }

    /* 3 Maker note */
    /* Clear previous debugInfo data */
    memset((void *)debugInfo.debugData[APP_MARKER_4], 0, debugInfo.debugSize[APP_MARKER_4]);
    /* back-up udm info for exif's maker note */
    memcpy((void *)debugInfo.debugData[APP_MARKER_4], (void *)udm, sizeof(struct camera2_udm));
    size_t offset = sizeof(struct camera2_udm);

#ifdef SAMSUNG_DUAL_CAPTURE_SOLUTION
    if (udm2) {
        memcpy((void *)(debugInfo.debugData[APP_MARKER_4] + offset), (void *)udm2, sizeof(struct camera2_udm));
        offset += sizeof(struct camera2_udm);
    }
#endif

#ifdef SAMSUNG_OIS
    if (getCameraId() == CAMERA_ID_BACK) {
        m_staticInfoExifLock.lock();
        getOisEXIFFromFile(m_staticInfo, (int)m_cameraInfo.oisMode);
        /* Copy ois data to debugData*/
        memcpy((void *)(debugInfo.debugData[APP_MARKER_4] + offset),
                (void *)&m_staticInfo->ois_exif_info, sizeof(m_staticInfo->ois_exif_info));

        offset += sizeof(m_staticInfo->ois_exif_info);
        m_staticInfoExifLock.unlock();
    }
#endif

#ifdef SAMSUNG_MTF
    m_staticInfoExifLock.lock();
    getMTFdataEXIFFromFile(m_staticInfo, getCameraId());
    /* Copy mtf data to debugData*/
    memcpy((void *)(debugInfo.debugData[APP_MARKER_4] + offset),
            (void *)&m_staticInfo->mtf_exif_info, sizeof(m_staticInfo->mtf_exif_info));

    offset += sizeof(m_staticInfo->mtf_exif_info);
    m_staticInfoExifLock.unlock();
#endif

// Check debug_attribute_t struct in ExynosExif.h
#ifdef SAMSUNG_LLS_DEBLUR
    int llsMode = getLDCaptureMode();
    if (llsMode == MULTI_SHOT_MODE_MULTI1
        || llsMode == MULTI_SHOT_MODE_MULTI2
        || llsMode == MULTI_SHOT_MODE_MULTI3) {
        memcpy((void *)(debugInfo.debugData[APP_MARKER_4] + offset),
                (void *)&m_staticInfo->lls_exif_info, sizeof(m_staticInfo->lls_exif_info));
        offset += sizeof(m_staticInfo->lls_exif_info);
    }
#endif

#ifdef SAMSUNG_LENS_DC
    if (getLensDCEnable()) {
        memcpy((void *)(debugInfo.debugData[APP_MARKER_4] + offset),
            (void *)&m_staticInfo->ldc_exif_info, sizeof(m_staticInfo->ldc_exif_info));
        offset += sizeof(m_staticInfo->ldc_exif_info);
    }
#endif
#ifdef SAMSUNG_STR_CAPTURE
    if (getSTRCaptureEnable()) {
        memcpy((void *)(debugInfo.debugData[APP_MARKER_4] + offset),
            (void *)&m_staticInfo->str_exif_info, sizeof(m_staticInfo->str_exif_info));
        offset += sizeof(m_staticInfo->str_exif_info);
    }
#endif

    int realDataSize = 0;
    m_staticInfoExifLock.lock();
    getSensorIdEXIFFromFile(m_staticInfo, getCameraId(), &realDataSize);
    /* Copy sensorId data to debugData*/
    memcpy((void *)(debugInfo.debugData[APP_MARKER_4] + offset),
           (void *)&m_staticInfo->sensor_id_exif_info, realDataSize);
    offset += realDataSize;
    m_staticInfoExifLock.unlock();

#ifdef SAMSUNG_UNI_API
    if (useDebugInfo2) {
        unsigned int appMarkerSize = (unsigned int)uni_appMarker_getSize(APP_MARKER_5);
        if (debugInfo.debugSize[APP_MARKER_5] > 0) {
            memset(debugInfo.debugData[APP_MARKER_5], 0, debugInfo.debugSize[APP_MARKER_5]);
        }
        if (appMarkerSize > 0 && debugInfo.debugSize[APP_MARKER_5] >= appMarkerSize) {
            char *flattenData = new char[appMarkerSize + 1];
            memset(flattenData, 0, appMarkerSize + 1);
            uni_appMarker_flatten(flattenData, APP_MARKER_5);
            memcpy(debugInfo.debugData[APP_MARKER_5], flattenData, appMarkerSize);
            delete[] flattenData;
        }
    }
#endif

#ifdef BURST_CAPTURE
    if (getSeriesShotCount() && getSeriesShotMode() == SERIES_SHOT_MODE_BURST) {
        unsigned char l_makernote[98] = { 0x07, 0x00, 0x01, 0x00, 0x07, 0x00, 0x04, 0x00, 0x00, 0x00,
                                          0x30, 0x31, 0x30, 0x30, 0x02, 0x00, 0x04, 0x00, 0x01, 0x00,
                                          0x00, 0x00, 0x00, 0x20, 0x01, 0x00, 0x40, 0x00, 0x04, 0x00,
                                          0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00,
                                          0x04, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x10, 0x00, 0x05, 0x00, 0x01, 0x00, 0x00, 0x00, 0x5A, 0x00,
                                          0x00, 0x00, 0x50, 0x00, 0x04, 0x00, 0x01, 0x00, 0x00, 0x00,
                                          0x01, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03, 0x00, 0x01, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
        exifInfo->maker_note_size = 98;
        memcpy(exifInfo->maker_note, l_makernote, sizeof(l_makernote));
    } else
#endif
    {
        exifInfo->maker_note_size = 0;
    }

    /* 3 Date time */
    struct timeval rawtime;
    struct tm timeinfo;
    gettimeofday(&rawtime, NULL);
    localtime_r((time_t *)&rawtime.tv_sec, &timeinfo);
    strftime((char *)exifInfo->date_time, 20, "%Y:%m:%d %H:%M:%S", &timeinfo);
    snprintf((char *)exifInfo->sec_time, 5, "%04d", (int)(rawtime.tv_usec/1000));

    /* 2 0th IFD Exif Private Tags */
    /*
     * vendorSpecific2[100]      : exposure
     * vendorSpecific2[101]      : iso(gain)
     * vendorSpecific2[102] /256 : Bv
     * vendorSpecific2[103]      : Tv
     */

    /* 3 ISO Speed Rating */
    exifInfo->iso_speed_rating = udm->internal.vendorSpecific2[101];

    /* 3 Exposure Program */
    if (m_exposureTimeCapture == 0) {
        exifInfo->exposure_program = EXIF_DEF_EXPOSURE_PROGRAM;
    } else {
        exifInfo->exposure_program = EXIF_DEF_EXPOSURE_MANUAL;
    }

    /* 3 Exposure Time */
    if (m_exposureTimeCapture == 0) {
        exifInfo->exposure_time.num = 1;

        if (udm->ae.vendorSpecific[0] == 0xAEAEAEAE)
            exifInfo->exposure_time.den = (uint32_t)udm->ae.vendorSpecific[64];
        else
            exifInfo->exposure_time.den = (uint32_t)udm->internal.vendorSpecific2[100];
    } else if (m_exposureTimeCapture > 0 && m_exposureTimeCapture <= CAMERA_PREVIEW_EXPOSURE_TIME_LIMIT) {
        exifInfo->exposure_time.num = 1;
        if ((1000000 % m_exposureTimeCapture) != 0) {
            uint32_t dig;
            if (m_exposureTimeCapture < 100) {
                dig = 1000;
            } else if (m_exposureTimeCapture < 1000) {
                dig = 100;
            } else if (m_exposureTimeCapture < 10000) {
                dig = 10;
            } else {
                dig = 5;
            }
            exifInfo->exposure_time.den = ROUND_OFF_DIGIT((int)(1000000 / m_exposureTimeCapture), dig);
        } else {
            exifInfo->exposure_time.den = (int)(1000000 / m_exposureTimeCapture);
        }
    } else {
        exifInfo->exposure_time.num = (uint32_t)m_exposureTimeCapture / 1000;
        exifInfo->exposure_time.den = 1000;
    }

    /* 3 Shutter Speed */
    if (m_exposureTimeCapture <= CAMERA_PREVIEW_EXPOSURE_TIME_LIMIT) {
        if (udm->ae.vendorSpecific[0] == 0xAEAEAEAE)
            exifInfo->shutter_speed.num = (uint32_t)(ROUND_OFF_HALF(((double)(udm->ae.vendorSpecific[69] / 256.f) * EXIF_DEF_APEX_DEN), 0));
        else
            exifInfo->shutter_speed.num = (uint32_t)(ROUND_OFF_HALF(((double)(udm->internal.vendorSpecific2[103] / 256.f) * EXIF_DEF_APEX_DEN), 0));

        exifInfo->shutter_speed.den = EXIF_DEF_APEX_DEN;
    } else {
        exifInfo->shutter_speed.num = (int32_t)(log2((double)m_exposureTimeCapture / 1000000) * -100);
        exifInfo->shutter_speed.den = 100;
    }

    /* 3 Aperture */
    exifInfo->aperture.num = APEX_FNUM_TO_APERTURE((double)(exifInfo->fnumber.num) / (double)(exifInfo->fnumber.den)) * m_staticInfo->apertureDen;
    exifInfo->aperture.den = m_staticInfo->apertureDen;

    /* 3 Max Aperture */
    exifInfo->max_aperture.num = APEX_FNUM_TO_APERTURE((double)(exifInfo->fnumber.num) / (double)(exifInfo->fnumber.den)) * m_staticInfo->apertureDen;
    exifInfo->max_aperture.den = m_staticInfo->apertureDen;

    /* 3 Brightness */
    int temp = udm->internal.vendorSpecific2[102];
    if ((int)udm->ae.vendorSpecific[102] < 0)
        temp = -temp;

    exifInfo->brightness.num = (int32_t)(ROUND_OFF_HALF((double)((temp * EXIF_DEF_APEX_DEN) / 256.f), 0));
    if ((int)udm->ae.vendorSpecific[102] < 0)
        exifInfo->brightness.num = -exifInfo->brightness.num;

    exifInfo->brightness.den = EXIF_DEF_APEX_DEN;

    CLOGD("udm->internal.vendorSpecific2[100](%d)", udm->internal.vendorSpecific2[100]);
    CLOGD("udm->internal.vendorSpecific2[101](%d)", udm->internal.vendorSpecific2[101]);
    CLOGD("udm->internal.vendorSpecific2[102](%d)", udm->internal.vendorSpecific2[102]);
    CLOGD("udm->internal.vendorSpecific2[103](%d)", udm->internal.vendorSpecific2[103]);

    CLOGD("iso_speed_rating(%d)", exifInfo->iso_speed_rating);
    CLOGD("exposure_time(%d/%d)", exifInfo->exposure_time.num, exifInfo->exposure_time.den);
    CLOGD("shutter_speed(%d/%d)", exifInfo->shutter_speed.num, exifInfo->shutter_speed.den);
    CLOGD("aperture     (%d/%d)", exifInfo->aperture.num, exifInfo->aperture.den);
    CLOGD("brightness   (%d/%d)", exifInfo->brightness.num, exifInfo->brightness.den);

    /* 3 Exposure Bias */
    if (m_exposureTimeCapture == 0) {
        exifInfo->exposure_bias.num = (int32_t)getExposureCompensation() * (m_staticInfo->exposureCompensationStep * 10);
        exifInfo->exposure_bias.den = 10;
    } else {
        exifInfo->exposure_bias.num = 0;
        exifInfo->exposure_bias.den = 1;
    }

    /* 3 Metering Mode */
#ifdef SAMSUNG_COMPANION
    enum companion_wdr_mode wdr_mode;
    wdr_mode = getRTHdr();
    if (wdr_mode == COMPANION_WDR_ON) {
        exifInfo->metering_mode = EXIF_METERING_PATTERN;
    } else
#endif
    {
        switch (m_cameraInfo.meteringMode) {
        case METERING_MODE_CENTER:
            exifInfo->metering_mode = EXIF_METERING_CENTER;
            break;
        case METERING_MODE_MATRIX:
            exifInfo->metering_mode = EXIF_METERING_AVERAGE;
            break;
        case METERING_MODE_SPOT:
#ifdef TOUCH_AE
        case METERING_MODE_CENTER_TOUCH:
        case METERING_MODE_SPOT_TOUCH:
        case METERING_MODE_AVERAGE_TOUCH:
        case METERING_MODE_MATRIX_TOUCH:
#endif
            exifInfo->metering_mode = EXIF_METERING_SPOT;
            break;
        case METERING_MODE_AVERAGE:
        default:
            exifInfo->metering_mode = EXIF_METERING_AVERAGE;
            break;
        }

#ifdef SAMSUNG_FOOD_MODE
        if(getSceneMode() == SCENE_MODE_FOOD) {
            exifInfo->metering_mode = EXIF_METERING_AVERAGE;
        }
#endif
    }

    /* 3 Flash */
    if (m_cameraInfo.flashMode == FLASH_MODE_OFF) {
        exifInfo->flash = 0;
    } else if (m_cameraInfo.flashMode == FLASH_MODE_TORCH) {
        exifInfo->flash = 1;
    } else {
        exifInfo->flash = getMarkingOfExifFlash();
    }

    /* 3 White Balance */
    if (m_cameraInfo.whiteBalanceMode == WHITE_BALANCE_AUTO)
        exifInfo->white_balance = EXIF_WB_AUTO;
    else
        exifInfo->white_balance = EXIF_WB_MANUAL;

    /* 3 Focal Length in 35mm length */
    exifInfo->focal_length_in_35mm_length = m_staticInfo->focalLengthIn35mmLength;

    /* 3 Scene Capture Type */
    switch (m_cameraInfo.sceneMode) {
    case SCENE_MODE_PORTRAIT:
        exifInfo->scene_capture_type = EXIF_SCENE_PORTRAIT;
        break;
    case SCENE_MODE_LANDSCAPE:
        exifInfo->scene_capture_type = EXIF_SCENE_LANDSCAPE;
        break;
    case SCENE_MODE_NIGHT:
        exifInfo->scene_capture_type = EXIF_SCENE_NIGHT;
        break;
    default:
        exifInfo->scene_capture_type = EXIF_SCENE_STANDARD;
        break;
    }

    /* 3 Image Unique ID */
#if defined(SAMSUNG_TN_FEATURE) && defined(SENSOR_FW_GET_FROM_FILE)
    char *front_fw = NULL;
    char *rear2_fw = NULL;
    char *savePtr = NULL;

    if (getCameraId() == CAMERA_ID_BACK){
        memset(exifInfo->unique_id, 0, sizeof(exifInfo->unique_id));
        strncpy((char *)exifInfo->unique_id,
                getSensorFWFromFile(m_staticInfo, m_cameraId), sizeof(exifInfo->unique_id) - 1);
    } else if (getCameraId() == CAMERA_ID_FRONT || getCameraId() == CAMERA_ID_FRONT) {
        front_fw = strtok_r((char *)getSensorFWFromFile(m_staticInfo, m_cameraId), " ", &savePtr);
        strcpy((char *)exifInfo->unique_id, front_fw);
    } else if (getCameraId() == CAMERA_ID_BACK_1) {
        rear2_fw = strtok_r((char *)getSensorFWFromFile(m_staticInfo, m_cameraId), " ", &savePtr);
        strcpy((char *)exifInfo->unique_id, rear2_fw);
    }
#endif

    /* 2 0th IFD GPS Info Tags */
    if (m_cameraInfo.gpsLatitude != 0 && m_cameraInfo.gpsLongitude != 0) {
        if (m_cameraInfo.gpsLatitude > 0)
            strncpy((char *)exifInfo->gps_latitude_ref, "N", 2);
        else
            strncpy((char *)exifInfo->gps_latitude_ref, "S", 2);

        if (m_cameraInfo.gpsLongitude > 0)
            strncpy((char *)exifInfo->gps_longitude_ref, "E", 2);
        else
            strncpy((char *)exifInfo->gps_longitude_ref, "W", 2);

        if (m_cameraInfo.gpsAltitude > 0)
            exifInfo->gps_altitude_ref = 0;
        else
            exifInfo->gps_altitude_ref = 1;

        double latitude = fabs(m_cameraInfo.gpsLatitude);
        double longitude = fabs(m_cameraInfo.gpsLongitude);
        double altitude = fabs(m_cameraInfo.gpsAltitude);

        exifInfo->gps_latitude[0].num = (uint32_t)latitude;
        exifInfo->gps_latitude[0].den = 1;
        exifInfo->gps_latitude[1].num = (uint32_t)((latitude - exifInfo->gps_latitude[0].num) * 60);
        exifInfo->gps_latitude[1].den = 1;
        exifInfo->gps_latitude[2].num = (uint32_t)(round((((latitude - exifInfo->gps_latitude[0].num) * 60)
                                        - exifInfo->gps_latitude[1].num) * 60));
        exifInfo->gps_latitude[2].den = 1;

        exifInfo->gps_longitude[0].num = (uint32_t)longitude;
        exifInfo->gps_longitude[0].den = 1;
        exifInfo->gps_longitude[1].num = (uint32_t)((longitude - exifInfo->gps_longitude[0].num) * 60);
        exifInfo->gps_longitude[1].den = 1;
        exifInfo->gps_longitude[2].num = (uint32_t)(round((((longitude - exifInfo->gps_longitude[0].num) * 60)
                                        - exifInfo->gps_longitude[1].num) * 60));
        exifInfo->gps_longitude[2].den = 1;

        exifInfo->gps_altitude.num = (uint32_t)altitude;
        exifInfo->gps_altitude.den = 1;

        struct tm tm_data;
        gmtime_r(&m_cameraInfo.gpsTimeStamp, &tm_data);
        exifInfo->gps_timestamp[0].num = tm_data.tm_hour;
        exifInfo->gps_timestamp[0].den = 1;
        exifInfo->gps_timestamp[1].num = tm_data.tm_min;
        exifInfo->gps_timestamp[1].den = 1;
        exifInfo->gps_timestamp[2].num = tm_data.tm_sec;
        exifInfo->gps_timestamp[2].den = 1;
        snprintf((char*)exifInfo->gps_datestamp, sizeof(exifInfo->gps_datestamp),
                "%04d:%02d:%02d", tm_data.tm_year + 1900, tm_data.tm_mon + 1, tm_data.tm_mday);

        exifInfo->enableGps = true;
    } else {
        exifInfo->enableGps = false;
    }

    /* 2 1th IFD TIFF Tags */
    exifInfo->widthThumb = thumbnailRect->w;
    exifInfo->heightThumb = thumbnailRect->h;
}

#ifdef SAMSUNG_DNG
status_t ExynosCamera1Parameters::checkDngFilePath(const CameraParameters& params)
{
    const char *dngFilePath = params.get("capture-raw-filepath");

    if (dngFilePath != NULL) {
        snprintf(m_dngFilePath, sizeof(m_dngFilePath), "%s", dngFilePath);
        CLOGD("[setParameters] dngFilePath %s", dngFilePath);
    } else {
        CLOGD("[setParameters] dngFilePath NULL");
        memset(m_dngFilePath, 0, CAMERA_FILE_PATH_SIZE);
    }
#if 0 /* bring-up : build error */
#ifdef USE_FW_ZOOMRATIO
#ifdef USE_DUAL_CAMERA
    meta_shot_ext->shot.uctl.zoomRatio = m_metadata.shot.uctl.zoomRatio;
#else
    if(getCameraId() == CAMERA_ID_BACK) {
        meta_shot_ext->shot.uctl.zoomRatio = m_metadata.shot.uctl.zoomRatio;
    }
#endif // USE_DUAL_CAMERA
#endif // USE_FW_ZOOMRATIO
#endif
    return NO_ERROR;
}

char *ExynosCamera1Parameters::getDngFilePath(void)
{
    return m_dngFilePath;
}

int ExynosCamera1Parameters::getDngSaveLocation(void)
{
    int dngShotSaveLocation = m_dngSaveLocation;

    /* GED's series shot work as callback */
#ifdef CAMERA_GED_FEATURE
    dngShotSaveLocation = BURST_SAVE_CALLBACK;
#else
    if (m_dngSaveLocation == 0)
        dngShotSaveLocation = BURST_SAVE_PHONE;
    else
        dngShotSaveLocation = BURST_SAVE_SDCARD;
#endif

    return dngShotSaveLocation;
}

void ExynosCamera1Parameters::setDngSaveLocation(int saveLocation)
{
    m_dngSaveLocation = saveLocation;
}

void ExynosCamera1Parameters::m_setDngFixedAttribute(void)
{
    char property[PROPERTY_VALUE_MAX];
    int32_t calibrationRG = 1024, calibrationBG = 1024;

    memset(&m_dngInfo, 0, sizeof(m_dngInfo));

    /* IFD TIFF Tags */
    /* string Tags */
    strncpy((char *)m_dngInfo.make, TIFF_DEF_MAKE, sizeof(m_dngInfo.make) - 1);
    m_dngInfo.make[sizeof(TIFF_DEF_MAKE) - 1] = '\0';
    property_get("ro.product.model", property, TIFF_DEF_MODEL);
    strncpy((char *)m_dngInfo.model, property, sizeof(m_dngInfo.model) - 1);
    m_dngInfo.model[sizeof(m_dngInfo.model) - 1] = '\0';
    strncpy((char *)m_dngInfo.unique_camera_model, property, sizeof(m_dngInfo.unique_camera_model) - 1);
    m_dngInfo.unique_camera_model[sizeof(m_dngInfo.unique_camera_model) - 1] = '\0';
    property_get("ro.build.PDA", property, TIFF_DEF_SOFTWARE);
    strncpy((char *)m_dngInfo.software, property, sizeof(m_dngInfo.software) - 1);
    m_dngInfo.software[sizeof(m_dngInfo.software) - 1] = '\0';

    /* pre-defined Tasgs at the header */
    m_dngInfo.new_subfile_type = 0;
    m_dngInfo.bits_per_sample = TIFF_DEF_BITS_PER_SAMPLE;
    m_dngInfo.rows_per_strip = TIFF_DEF_ROWS_PER_STRIP;
    m_dngInfo.photometric_interpretation = TIFF_DEF_PHOTOMETRIC_INTERPRETATION;
    m_dngInfo.image_description[0] = '\0';
    m_dngInfo.planar_configuration = TIFF_DEF_PLANAR_CONFIGURATION;
    m_dngInfo.copyright = '\0';
    m_dngInfo.samples_per_pixel = TIFF_DEF_SAMPLES_PER_PIXEL;
    m_dngInfo.compression = TIFF_DEF_COMPRESSION;
    m_dngInfo.x_resolution.num = TIFF_DEF_RESOLUTION_NUM;
    m_dngInfo.x_resolution.den = TIFF_DEF_RESOLUTION_DEN;
    m_dngInfo.y_resolution.num = TIFF_DEF_RESOLUTION_NUM;
    m_dngInfo.y_resolution.den = TIFF_DEF_RESOLUTION_DEN;
    m_dngInfo.resolution_unit = TIFF_DEF_RESOLUTION_UNIT;
    m_dngInfo.cfa_layout = TIFF_DEF_CFA_LAYOUT;
    memcpy(m_dngInfo.tiff_ep_standard_id, TIFF_DEF_TIFF_EP_STANDARD_ID, sizeof(m_dngInfo.tiff_ep_standard_id));
    memcpy(m_dngInfo.cfa_repeat_pattern_dm, TIFF_DEF_CFA_REPEAT_PATTERN_DM, sizeof(m_dngInfo.cfa_repeat_pattern_dm));
    memcpy(m_dngInfo.cfa_pattern, TIFF_DEF_CFA_PATTERN, sizeof(m_dngInfo.cfa_pattern));
    memcpy(m_dngInfo.dng_version, TIFF_DEF_DNG_VERSION, sizeof(m_dngInfo.dng_version));
    memcpy(m_dngInfo.dng_backward_version, TIFF_DEF_DNG_BACKWARD_VERSION, sizeof(m_dngInfo.dng_backward_version));
    memcpy(m_dngInfo.cfa_plane_color, TIFF_DEF_CFA_PLANE_COLOR, sizeof(m_dngInfo.cfa_plane_color));
    memcpy(m_dngInfo.black_level_repeat_dim, TIFF_DEF_BLACK_LEVEL_REPEAT_DIM, sizeof(m_dngInfo.black_level_repeat_dim));
    memcpy(m_dngInfo.default_scale, TIFF_DEF_DEFAULT_SCALE, sizeof(m_dngInfo.default_scale));
    memcpy(m_dngInfo.opcode_list2, &TIFF_DEF_OPCODE_LIST2, sizeof(m_dngInfo.opcode_list2));
    memcpy(m_dngInfo.exif_version, &TIFF_DEF_EXIF_VERSION, sizeof(m_dngInfo.exif_version));
    memcpy(m_dngInfo.thumbnail_bits_per_sample, &TIFF_DEF_THUMB_BIT_PER_SAMPLE,
            sizeof(m_dngInfo.thumbnail_bits_per_sample));

    /* static Metadata */
    m_dngInfo.default_crop_origin[0] = m_staticInfo->sensorMarginW;
    m_dngInfo.default_crop_origin[1] = m_staticInfo->sensorMarginH;
    m_dngInfo.f_number.num = m_staticInfo->fNumberNum;
    m_dngInfo.f_number.den = m_staticInfo->fNumberDen;
    m_dngInfo.focal_length.num = m_staticInfo->focalLengthNum;
    m_dngInfo.focal_length.den = m_staticInfo->focalLengthDen;
    m_dngInfo.white_level = m_staticInfo->whiteLevel;
    m_dngInfo.calibration_illuminant1 = m_staticInfo->referenceIlluminant1;
    m_dngInfo.calibration_illuminant2 = m_staticInfo->referenceIlluminant2;
    memcpy(m_dngInfo.black_level_repeat, m_staticInfo->blackLevelPattern, sizeof(m_dngInfo.black_level_repeat));
    memcpy(m_dngInfo.color_matrix1, m_staticInfo->colorTransformMatrix1, sizeof(m_dngInfo.color_matrix1));
    memcpy(m_dngInfo.color_matrix2, m_staticInfo->colorTransformMatrix2, sizeof(m_dngInfo.color_matrix2));
    memcpy(m_dngInfo.forward_matrix1, m_staticInfo->forwardMatrix1, sizeof(m_dngInfo.forward_matrix1));
    memcpy(m_dngInfo.forward_matrix2, m_staticInfo->forwardMatrix2, sizeof(m_dngInfo.forward_matrix2));

    getAWBCalibrationGain(&calibrationRG, &calibrationBG, m_staticInfo->masterRGain, m_staticInfo->masterBGain);
    m_staticInfo->calibration1[0].numerator = calibrationRG;
    m_staticInfo->calibration1[8].numerator = calibrationBG;
    m_staticInfo->calibration2[0].numerator = calibrationRG;
    m_staticInfo->calibration2[8].numerator = calibrationBG;
    memcpy(m_dngInfo.camera_calibration1, m_staticInfo->calibration1, sizeof(m_dngInfo.camera_calibration1));
    memcpy(m_dngInfo.camera_calibration2, m_staticInfo->calibration2, sizeof(m_dngInfo.camera_calibration2));
}

void ExynosCamera1Parameters::setDngChangedAttribute(struct camera2_dm *dm, struct camera2_udm *udm)
{
    CLOGD(" set Dynamic Dng Info");

    /* IFD TIFF Tags */
    getHwSensorSize((int *)&m_dngInfo.image_width, (int *)&m_dngInfo.image_height);
    getThumbnailSize((int *)&m_dngInfo.thumbnail_image_width, (int *)&m_dngInfo.thumbnail_image_height);
    m_dngInfo.thumbnail_rows_per_strip = m_dngInfo.thumbnail_image_height;
    m_dngInfo.default_crop_size[0] = m_dngInfo.image_width - m_dngInfo.default_crop_origin[0];
    m_dngInfo.default_crop_size[1] = m_dngInfo.image_height - m_dngInfo.default_crop_origin[1];
    m_dngInfo.thumbnail_offset = DNG_HEADER_FILE_SIZE + (m_dngInfo.image_width * m_dngInfo.image_height * 2);

    /* Orientation */
    switch (m_cameraInfo.rotation) {
        case 90:
            m_dngInfo.orientation = EXIF_ORIENTATION_90;
            break;
        case 180:
            m_dngInfo.orientation = EXIF_ORIENTATION_180;
            break;
        case 270:
            m_dngInfo.orientation = EXIF_ORIENTATION_270;
            break;
        case 0:
        default:
            m_dngInfo.orientation = EXIF_ORIENTATION_UP;
            break;
    }

    /* Date time */
    struct timeval rawtime;
    struct tm timeinfo;
    gettimeofday(&rawtime, NULL);
    localtime_r((time_t *)&rawtime.tv_sec, &timeinfo);
    strftime((char *)m_dngInfo.date_time, 20, "%Y:%m:%d %H:%M:%S", &timeinfo);
    strftime((char *)m_dngInfo.date_time_original, 20, "%Y:%m:%d %H:%M:%S", &timeinfo);

    /* ISO Speed Rating */
    m_dngInfo.iso_speed_ratings = udm->internal.vendorSpecific2[101];

    /* Exposure Time */
    if (m_exposureTimeCapture == 0) {
        m_dngInfo.exposure_time.num = 1;

        if (udm->ae.vendorSpecific[0] == 0xAEAEAEAE)
            m_dngInfo.exposure_time.den = (uint32_t)udm->ae.vendorSpecific[64];
        else
            m_dngInfo.exposure_time.den = (uint32_t)udm->internal.vendorSpecific2[100];
    } else if (m_exposureTimeCapture > 0 && m_exposureTimeCapture <= CAMERA_PREVIEW_EXPOSURE_TIME_LIMIT) {
        m_dngInfo.exposure_time.num = 1;
        if ((1000000 % m_exposureTimeCapture) != 0) {
            uint32_t dig;
            if (m_exposureTimeCapture < 100) {
                dig = 1000;
            } else if (m_exposureTimeCapture < 1000) {
                dig = 100;
            } else if (m_exposureTimeCapture < 10000) {
                dig = 10;
            } else {
                dig = 5;
            }
            m_dngInfo.exposure_time.den = ROUND_OFF_DIGIT((int)(1000000 / m_exposureTimeCapture), dig);
        } else {
            m_dngInfo.exposure_time.den = (int)(1000000 / m_exposureTimeCapture);
        }
    } else {
        m_dngInfo.exposure_time.num = m_exposureTimeCapture / 1000;
        m_dngInfo.exposure_time.den = 1000;
    }

    /* AsShotNeutral & NoiseProfile */
    memcpy(m_dngInfo.as_shot_neutral, dm->sensor.neutralColorPoint, sizeof(m_dngInfo.as_shot_neutral));
    memcpy(m_dngInfo.noise_profile, dm->sensor.noiseProfile, sizeof(m_dngInfo.noise_profile));

    /* 3 Image Unique ID */
#if defined(SAMSUNG_TN_FEATURE) && defined(SENSOR_FW_GET_FROM_FILE)
    char *front_fw = NULL;
    char *rear2_fw = NULL;
    char *savePtr = NULL;

    if (getCameraId() == CAMERA_ID_BACK){
        memset(m_dngInfo.unique_camera_model, 0, sizeof(m_dngInfo.unique_camera_model));
        strncpy((char *)m_dngInfo.unique_camera_model, getSensorFWFromFile(m_staticInfo, m_cameraId),
                sizeof(m_dngInfo.unique_camera_model) - 1);
    } else if (getCameraId() == CAMERA_ID_FRONT || getCameraId() == CAMERA_ID_FRONT_1) {
        front_fw = strtok_r((char *)getSensorFWFromFile(m_staticInfo, m_cameraId), " ", &savePtr);
        strcpy((char *)m_dngInfo.unique_camera_model, front_fw);
    } else if (getCameraId() == CAMERA_ID_BACK_1) {
        rear2_fw = strtok_r((char *)getSensorFWFromFile(m_staticInfo, m_cameraId), " ", &savePtr);
        strcpy((char *)m_dngInfo.unique_camera_model, rear2_fw);
    }
#endif
}

dng_attribute_t *ExynosCamera1Parameters::getDngInfo()
{
    int retryCount = 12; /* 30ms * 12 */
    while(retryCount > 0) {
        if(getIsUsefulDngInfo() == false) {
            CLOGD(" Waiting for update DNG metadata failed, retryCount(%d)", retryCount);
        } else {
            CLOGD(" Success DNG meta, retryCount(%d)", retryCount);
            break;
        }
        retryCount--;
        usleep(DM_WAITING_TIME);
    }

    return &m_dngInfo;
}

void ExynosCamera1Parameters::setIsUsefulDngInfo(bool enable)
{
    m_isUsefulDngInfo = enable;
}

bool ExynosCamera1Parameters::getIsUsefulDngInfo()
{
    return m_isUsefulDngInfo;
}

dng_thumbnail_t *ExynosCamera1Parameters::createDngThumbnailBuffer(int size)
{
    return m_dngThumbnail.createNode(size);
}

dng_thumbnail_t *ExynosCamera1Parameters::putDngThumbnailBuffer(dng_thumbnail_t *buf)
{
    return m_dngThumbnail.putNode(buf);
}

dng_thumbnail_t *ExynosCamera1Parameters::getDngThumbnailBuffer(unsigned int frameCount)
{
    return m_dngThumbnail.getNode(frameCount);
}

void ExynosCamera1Parameters::deleteDngThumbnailBuffer(dng_thumbnail_t *curNode)
{
    m_dngThumbnail.deleteNode(curNode);
}

void ExynosCamera1Parameters::cleanDngThumbnailBuffer()
{
    m_dngThumbnail.cleanNode();
}
#endif

#ifdef SAMSUNG_QUICKSHOT
status_t ExynosCamera1Parameters::checkQuickShot(const CameraParameters& params)
{
    int newQshot = params.getInt("quick-shot");
    int curQshot= -1;

    CLOGD("[setParameters]newQshot %d", newQshot);

    if (newQshot < 0) {
        CLOGE(" Invalid quick shot mode %d", newQshot);
        return BAD_VALUE;
    }

    curQshot = getQuickShot();

    if (curQshot != newQshot) {
        m_params.set("quick-shot", newQshot);
    }

    return NO_ERROR;
}

int ExynosCamera1Parameters::getQuickShot(void)
{
    int32_t qshot = 0;

    qshot = m_params.getInt("quick-shot");
    return qshot;
}
#endif

#ifdef SAMSUNG_OIS
status_t ExynosCamera1Parameters::checkOIS(const CameraParameters& params)
{
    enum optical_stabilization_mode newOIS = OPTICAL_STABILIZATION_MODE_OFF;
    enum optical_stabilization_mode curOIS = OPTICAL_STABILIZATION_MODE_OFF;
    const char *strNewOIS = params.get("ois");
    int zoomLevel = getZoomLevel();
    int zoomRatio = (int)getZoomRatio(zoomLevel);

    if (strNewOIS == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewOIS %s", strNewOIS);

#ifdef SAMSUNG_OIS_VDIS
    if (strcmp(strNewOIS,"vdis")) {
        setSWVdisOISInit();
    }
#endif

    if (!strcmp(strNewOIS, "off")) {
        newOIS = OPTICAL_STABILIZATION_MODE_OFF;
    } else if (!strcmp(strNewOIS, "still")) {
        if (getRecordingHint() == true) {
            newOIS = OPTICAL_STABILIZATION_MODE_VIDEO;
        } else {
            if(zoomRatio >= 4000) {
                newOIS = OPTICAL_STABILIZATION_MODE_STILL_ZOOM;
            } else {
                newOIS = OPTICAL_STABILIZATION_MODE_STILL;
            }
        }
    } else if (!strcmp(strNewOIS, "still_zoom")) {
        newOIS = OPTICAL_STABILIZATION_MODE_STILL_ZOOM;
    } else if (!strcmp(strNewOIS, "video")) {
        newOIS = OPTICAL_STABILIZATION_MODE_VIDEO;
    } else if (!strcmp(strNewOIS, "sine_x")) {
        newOIS = OPTICAL_STABILIZATION_MODE_SINE_X;
    } else if (!strcmp(strNewOIS, "sine_y")) {
        newOIS = OPTICAL_STABILIZATION_MODE_SINE_Y;
    } else if (!strcmp(strNewOIS, "center")) {
        newOIS = OPTICAL_STABILIZATION_MODE_CENTERING;
    }
#ifdef SAMSUNG_OIS_VDIS
    else if (!strcmp(strNewOIS, "vdis")) {
        uint32_t oisGain = getOISGain();
        setOISCoef(oisGain);
        newOIS = OPTICAL_STABILIZATION_MODE_VDIS;
    }
#endif
    else {
        CLOGE("Invalid ois command(%s)", strNewOIS);
        return BAD_VALUE;
    }

    curOIS = getOIS();

    if (curOIS != newOIS) {
        CLOGD("set OIS, new OIS Mode = %d", newOIS);
        m_setOIS(newOIS);
        m_params.set("ois", strNewOIS);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setOIS(enum  optical_stabilization_mode ois)
{
    m_cameraInfo.oisMode = ois;

    if(getZoomActiveOn()) {
        CLOGD("[setParameters]zoom moving..");
        return;
    }

#if 0 // Host controlled OIS Factory Mode
    if(m_oisNode) {
        setOISMode();
    } else {
        CLOGD("Ois node is not prepared yet!!!!");
        m_setOISmodeSetting = true;
    }
#else
    setOISMode();
#endif
}

enum optical_stabilization_mode ExynosCamera1Parameters::getOIS(void)
{
    return m_cameraInfo.oisMode;
}

void ExynosCamera1Parameters::setOISNode(ExynosCameraNode *node)
{
    m_oisNode = node;
}

void ExynosCamera1Parameters::setOISModeSetting(bool enable)
{
    m_setOISmodeSetting = enable;
}

int ExynosCamera1Parameters::getOISModeSetting(void)
{
    return m_setOISmodeSetting;
}

void ExynosCamera1Parameters::setOISMode(void)
{
    //int ret = 0;

    CLOGD("set OIS Mode = %d", m_cameraInfo.oisMode);

    setMetaCtlOIS(&m_metadata, m_cameraInfo.oisMode);

#if 0 // Host controlled OIS Factory Mode
    if (m_cameraInfo.oisMode == OPTICAL_STABILIZATION_MODE_SINE_X && m_oisNode != NULL) {
        ret = m_oisNode->setControl(V4L2_CID_CAMERA_OIS_SINE_MODE, OPTICAL_STABILIZATION_MODE_SINE_X);
        if (ret < 0) {
            CLOGE("FLITE setControl fail, ret(%d)", ret);
        }
    } else if (m_cameraInfo.oisMode == OPTICAL_STABILIZATION_MODE_SINE_Y && m_oisNode != NULL) {
        ret = m_oisNode->setControl(V4L2_CID_CAMERA_OIS_SINE_MODE, OPTICAL_STABILIZATION_MODE_SINE_Y);
        if (ret < 0) {
            CLOGE("FLITE setControl fail, ret(%d)", ret);
        }
    }
#endif
}

#endif

#ifdef SAMSUNG_COMPANION
status_t ExynosCamera1Parameters::checkRTDrc(const CameraParameters& params)
{
    enum companion_drc_mode newRTDrc = COMPANION_DRC_OFF;
    enum companion_drc_mode curRTDrc = COMPANION_DRC_OFF;
    const char *strNewRTDrc = params.get("dynamic-range-control");

    if (strNewRTDrc == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewRTDrc %s", strNewRTDrc);

    if (!strcmp(strNewRTDrc, "off")) {
        newRTDrc = COMPANION_DRC_OFF;
    } else if (!strcmp(strNewRTDrc, "on")) {
        newRTDrc = COMPANION_DRC_ON;
    } else {
        CLOGE("Invalid rt drc(%s)", strNewRTDrc);
        return BAD_VALUE;
    }

    curRTDrc = getRTDrc();

    if (curRTDrc != newRTDrc) {
        m_setRTDrc(newRTDrc);
        m_params.set("dynamic-range-control", strNewRTDrc);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setRTDrc(enum  companion_drc_mode rtdrc)
{
    setMetaCtlRTDrc(&m_metadata, rtdrc);
}

enum companion_drc_mode ExynosCamera1Parameters::getRTDrc(void)
{
    enum companion_drc_mode mode;

    getMetaCtlRTDrc(&m_metadata, &mode);

    return mode;
}

status_t ExynosCamera1Parameters::checkPaf(const CameraParameters& params)
{
    enum companion_paf_mode newPaf = COMPANION_PAF_OFF;
    enum companion_paf_mode curPaf = COMPANION_PAF_OFF;
    const char *strNewPaf = params.get("phase-af");

    if (strNewPaf == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewPaf %s", strNewPaf);

    if (!strcmp(strNewPaf, "off")) {
        newPaf = COMPANION_PAF_OFF;
    } else if (!strcmp(strNewPaf, "on")) {
        newPaf = COMPANION_PAF_ON;
    } else {
        CLOGE("Invalid paf(%s)", strNewPaf);
        return BAD_VALUE;
    }

#ifdef CAMERA_GED_FEATURE
    newPaf = (enum companion_paf_mode) 0; //no effect
#endif

    curPaf = getPaf();

    if (curPaf != newPaf) {
        m_setPaf(newPaf);
        m_params.set("phase-af", strNewPaf);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setPaf(enum companion_paf_mode paf)
{
    setMetaCtlPaf(&m_metadata, paf);
}

enum companion_paf_mode ExynosCamera1Parameters::getPaf(void)
{
    enum companion_paf_mode mode = COMPANION_PAF_OFF;

    getMetaCtlPaf(&m_metadata, &mode);

    return mode;
}

status_t ExynosCamera1Parameters::checkRTHdr(const CameraParameters& params)
{
    enum companion_wdr_mode newRTHdr = COMPANION_WDR_OFF;
    enum companion_wdr_mode curRTHdr = COMPANION_WDR_OFF;
    const char *strNewRTHdr = params.get("rt-hdr");

    if (strNewRTHdr == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewRTWdr %s", strNewRTHdr);

    if (!strcmp(strNewRTHdr, "off")) {
#ifdef SUPPORT_WDR_AUTO_LIKE
        if (getSamsungCamera() == true
            && (getCameraId() == CAMERA_ID_BACK || getCameraId() == CAMERA_ID_BACK_1)
            && !(getCaptureExposureTime() > 0 || getFastFpsMode() >= 2 || (getDualMode() && !getDualCameraMode()))
            && !(getShotMode() == SHOT_MODE_LIVE_OUTFOCUS)) {
            newRTHdr = COMPANION_WDR_AUTO_LIKE;
        } else
#endif
        {
            newRTHdr = COMPANION_WDR_OFF;
        }
    } else if (!strcmp(strNewRTHdr, "on")) {
        newRTHdr = COMPANION_WDR_ON;
    } else if (!strcmp(strNewRTHdr, "auto")) {
        newRTHdr = COMPANION_WDR_AUTO;
    } else {
        CLOGE("Invalid rt wdr(%s)", strNewRTHdr);
        return BAD_VALUE;
    }

    curRTHdr = getRTHdr();

    if (curRTHdr != newRTHdr) {
        m_setRTHdr(newRTHdr);
        m_params.set("rt-hdr", strNewRTHdr);
        /* For Samsung SDK */
        if (getPreviewRunning() == true
#ifdef SUPPORT_WDR_AUTO_LIKE
            && !((curRTHdr == COMPANION_WDR_OFF && newRTHdr == COMPANION_WDR_AUTO_LIKE)
                || (curRTHdr == COMPANION_WDR_AUTO_LIKE && newRTHdr == COMPANION_WDR_OFF))
#endif
        ) {
            CLOGD("setRestartPreviewChecked true");
            m_setRestartPreviewChecked(true);
        }
        m_flagChangedRtHdr = true;
    }

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::checkSceneRTHdr(bool onoff)
{
    if (onoff) {
        m_setRTHdr(COMPANION_WDR_ON);
        m_setRTDrc(COMPANION_DRC_ON);
    } else {
        m_setRTHdr(COMPANION_WDR_OFF);
        m_setRTDrc(COMPANION_DRC_OFF);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setRTHdr(enum companion_wdr_mode rthdr)
{
    setMetaCtlRTHdr(&m_metadata, rthdr);
}

enum companion_wdr_mode ExynosCamera1Parameters::getRTHdr(void)
{
    enum companion_wdr_mode mode = COMPANION_WDR_OFF;

    if (getUseCompanion() == true || getCameraId() == CAMERA_ID_BACK_1)
        getMetaCtlRTHdr(&m_metadata, &mode);

    return mode;
}

bool ExynosCamera1Parameters::getFlagChangedRtHdr(void)
{
    return m_flagChangedRtHdr;
}

void ExynosCamera1Parameters::setFlagChangedRtHdr(bool state)
{
    m_flagChangedRtHdr = state;
}
#endif

#ifdef SAMSUNG_TN_FEATURE
void ExynosCamera1Parameters::setParamExifInfo(camera2_udm *udm)
{
    uint32_t iso = udm->internal.vendorSpecific2[101];
    uint32_t exposure_time = 0;

    if (udm->ae.vendorSpecific[0] == 0xAEAEAEAE)
        exposure_time = udm->ae.vendorSpecific[64];
    else
        exposure_time = udm->internal.vendorSpecific2[100];

    CLOGI("set exif_exptime %u, exif_iso %u", exposure_time, iso);

#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true) {
        m_dualParameters->setParamExifInfo(CAMERA_ID_BACK, iso, exposure_time);
    } else
#endif
    {
    m_params.set("exif_exptime", exposure_time);
    m_params.set("exif_iso", iso);
    }
}
#endif

#ifdef SAMSUNG_LBP
int32_t ExynosCamera1Parameters::getHoldFrameCount(void)
{
    if(getCameraId() == CAMERA_ID_BACK)
        return MAX_LIVE_REAR_BEST_PICS;
    else
        return MAX_LIVE_FRONT_BEST_PICS;
}
#endif

#ifdef USE_BINNING_MODE
int ExynosCamera1Parameters::getBinningMode(void)
{
    int ret = 0;
    int vt_mode = getVtMode();
#ifdef USE_LIVE_BROADCAST
    int camera_id = getCameraId();
    bool plb_mode = getPLBMode();
#endif

    if (m_binningProperty == true)
        CLOGI("binning mode = %d", m_binningProperty);

    if (m_staticInfo->vtcallSizeLutMax == 0 || m_staticInfo->vtcallSizeLut == NULL) {
       CLOGV("vtCallSizeLut is NULL, can't support the binnig mode");
       return ret;
    }

#ifdef SAMSUNG_COMPANION
    if ((getCameraId() == CAMERA_ID_FRONT || getCameraId() == CAMERA_ID_FRONT || getCameraId() == CAMERA_ID_BACK_1) &&
        (getUseCompanion() == true)) {
        CLOGV(" Companion mode in front can't support the binning mode.(%d,%d)",
        getCameraId(), getUseCompanion());
        return ret;
    }
#endif

    /* For VT Call with DualCamera Scenario */
    if (getDualMode()
#ifdef USE_LIVE_BROADCAST_DUAL
        && (plb_mode != true || (plb_mode == true && (camera_id == CAMERA_ID_FRONT || camera_id == CAMERA_ID_FRONT_1)))
#endif
       ) {
        CLOGV("DualMode can't support the binnig mode.(%d,%d)", getCameraId(), getDualMode());
        return ret;
    }

    if (vt_mode != 3 && vt_mode > 0 && vt_mode < 5) {
        ret = 1;
    }
#ifdef USE_LIVE_BROADCAST
    else if (plb_mode == true) {
        if (camera_id == CAMERA_ID_BACK) {
            ret = 1;
        }
    }
#endif
    else {
        if (m_binningProperty == true
            && getSamsungCamera() == false) {
            ret = 1;
        }
    }
    return ret;
}
#endif

#ifdef USE_REMOSAIC_CAPTURE
int ExynosCamera1Parameters::getAvailableRemosaicCapture(void)
{
    int ret = 1;

    /* ToDo */
    /* Accordig to lux information, decide to use remosaic capture */

    /* lowlight condition check */

    if(this->getRecordingHint() == true)
        return 0;

    if(isUseRemosaicCapture() == false)
        return 0;

    return ret;
}

void ExynosCamera1Parameters::setRemosaicCaptureMode(enum remosaic_capture_mode enable) {
    m_flagRemosaicCaptureMode = enable;
}

int ExynosCamera1Parameters::getRemosaicCaptureMode() {

    if(isUseRemosaicCapture() == true)
        return m_flagRemosaicCaptureMode;
    else
        return REMOSAIC_CAPTURE_MODE_NOT_SUPPORTED;
}

bool ExynosCamera1Parameters::isUseRemosaicCapture() {
    bool ret = false;

    switch(getCameraId()) {
    case CAMERA_ID_FRONT:
        ret = USE_REMOSAIC_CAPTURE_FRONT;
        break;
    case CAMERA_ID_BACK:
        ret = USE_REMOSAIC_CAPTURE_BACK;
        break;
    default:
        ret = false;
        break;
    }

    return ret;
}
#endif

status_t ExynosCamera1Parameters::checkFlashMode(const CameraParameters& params)
{
    int newFlashMode = -1;
    int curFlashMode = -1;
    const char *strFlashMode = params.get(CameraParameters::KEY_FLASH_MODE);
    const char *strNewFlashMode = m_adjustFlashMode(strFlashMode);

    if (strNewFlashMode == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewFlashMode %s", strNewFlashMode);

    if (!strcmp(strNewFlashMode, CameraParameters::FLASH_MODE_OFF)) {
        newFlashMode = FLASH_MODE_OFF;
    } else if (!strcmp(strNewFlashMode, CameraParameters::FLASH_MODE_AUTO)) {
        newFlashMode = FLASH_MODE_AUTO;
    } else if (!strcmp(strNewFlashMode, CameraParameters::FLASH_MODE_ON)) {
        newFlashMode = FLASH_MODE_ON;
    } else if (!strcmp(strNewFlashMode, CameraParameters::FLASH_MODE_RED_EYE)) {
        newFlashMode = FLASH_MODE_RED_EYE;
    } else if (!strcmp(strNewFlashMode, CameraParameters::FLASH_MODE_TORCH)) {
        newFlashMode = FLASH_MODE_TORCH;
    } else {
        CLOGE("unmatched flash_mode(%s), turn off flash", strNewFlashMode);
        newFlashMode = FLASH_MODE_OFF;
        strNewFlashMode = CameraParameters::FLASH_MODE_OFF;
        return BAD_VALUE;
    }

#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true && newFlashMode != FLASH_MODE_TORCH) {
        int masterCameraId = -1;
        int slaveCameraId = -1;

        getDualCameraId(&masterCameraId, &slaveCameraId);

        switch(getDualCameraReprocessingSyncType()) {
        case SYNC_TYPE_BYPASS:
        case SYNC_TYPE_SYNC:
            if (m_cameraId == slaveCameraId) {
                CLOGD("skip flash control");
                newFlashMode = FLASH_MODE_OFF;
            }
            break;
        case SYNC_TYPE_SWITCH:
            if (m_cameraId == masterCameraId) {
                CLOGD("skip flash control");
                newFlashMode = FLASH_MODE_OFF;
            }
            break;
        default:
            CLOGE("invalid sync type(%d)", getDualCameraReprocessingSyncType());
            newFlashMode = FLASH_MODE_OFF;
            break;
        }
    }
#endif

#ifndef UNSUPPORT_FLASH
    if (!(newFlashMode & getSupportedFlashModes())) {
#ifdef SAMSUNG_FRONT_LCD_FLASH
        if ((getSamsungCamera() == true) && (m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1)) {
            CLOGD(" Flash mode is supported for LCD flash.");
        }
        else
#endif
        {
            CLOGE(" Flash mode(%s) is not supported!", strNewFlashMode);
            return BAD_VALUE;
        }
    }
#endif

    curFlashMode = getFlashMode();

    if (curFlashMode != newFlashMode) {
        m_setFlashMode(newFlashMode);
        m_params.set(CameraParameters::KEY_FLASH_MODE, strNewFlashMode);
    }
#ifdef USE_DUAL_CAMERA
    else if (getDualCameraMode() == true) {
        m_params.set(CameraParameters::KEY_FLASH_MODE, strNewFlashMode);
    }
#endif

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setFlashMode(int flashMode)
{
    m_cameraInfo.flashMode = flashMode;

    /* TODO: Notity flash activity */
    m_activityControl->setFlashMode(flashMode);

#ifdef USE_FW_FLASHMODE
    enum camera_flash_mode newFlashMode;

    switch(flashMode) {
    case FLASH_MODE_OFF:
        newFlashMode = CAMERA_FLASH_MODE_OFF;
        break;
    case FLASH_MODE_AUTO:
        newFlashMode = CAMERA_FLASH_MODE_AUTO;
        break;
    case FLASH_MODE_ON:
        newFlashMode = CAMERA_FLASH_MODE_ON;
        break;
    case FLASH_MODE_RED_EYE:
        newFlashMode = CAMERA_FLASH_MODE_RED_EYE;
        break;
    case FLASH_MODE_TORCH:
        newFlashMode = CAMERA_FLASH_MODE_TORCH;
        break;
    default:
        newFlashMode = CAMERA_FLASH_MODE_OFF;
        CLOGE("Flash mode(%d) is not supported", flashMode);
        break;
    }
    setMetaUctlFlashMode(&m_metadata, newFlashMode);
#endif
}

#ifdef FLASHED_LLS_CAPTURE
void ExynosCamera1Parameters::setFlashedLLSCapture(bool isLLSCapture)
{
    m_isFlashedLLSCapture = isLLSCapture;
}

bool ExynosCamera1Parameters::getFlashedLLSCapture()
{
    return m_isFlashedLLSCapture;
}
#endif

status_t ExynosCamera1Parameters::checkDualMode(const CameraParameters& params)
{
    /* dual_mode */
    bool flagDualMode = false;
    int newDualMode = params.getInt("dual_mode");

    if (newDualMode == 1) {
        CLOGD("[setParameters]newDualMode : %d", newDualMode);
        flagDualMode = true;
    }

#ifdef USE_DUAL_CAMERA
    if (m_flagCheckDualCameraMode == true) {
        if (getDualCameraMode() == true) {
            flagDualMode = true;
        }
    }
#endif

    setDualMode(flagDualMode);
    m_params.set("dual_mode", newDualMode);

    if (newDualMode == 1) {
        m_setDualShotMode(params);
    }

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::checkShotMode(const CameraParameters& params)
{
    int newShotMode = params.getInt("shot-mode");
    int orgShotMode = params.getInt("shot-mode");
    int curShotMode = -1;

    if (newShotMode < 0) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newShotMode %d", newShotMode);

    curShotMode = getShotMode();

    if ((getRecordingHint() == true)
#ifdef SAMSUNG_TN_FEATURE
        && (newShotMode != SHOT_MODE_SEQUENCE)
#endif
#ifdef SAMSUNG_HYPER_MOTION
        && (newShotMode != SHOT_MODE_HYPER_MOTION)
#endif
        ) {
        m_setShotMode(SHOT_MODE_NORMAL);
        m_params.set("shot-mode", SHOT_MODE_NORMAL);
    } else if (curShotMode != newShotMode) {
        m_setShotMode(newShotMode);
        m_params.set("shot-mode", orgShotMode);

        if (newShotMode == SHOT_MODE_FACE_LOCK
#ifdef SAMSUNG_COLOR_IRIS
            || newShotMode == SHOT_MODE_COLOR_IRIS
#endif
            ) {
            setUseFastenAeStable(true);
        }

        updatePreviewFpsRange();
    }
#ifdef LLS_CAPTURE
    else if (m_flagLLSOn == true) {
        m_setLLSShotMode(params);
    }
#endif

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setShotMode(int shotMode)
{
    enum aa_mode mode = AA_CONTROL_AUTO;
    enum aa_scene_mode sceneMode = AA_SCENE_MODE_FACE_PRIORITY;
    bool changeSceneMode = true;

    switch (shotMode) {
    case SHOT_MODE_DRAMA:
#ifdef SAMSUNG_DOF
    case SHOT_MODE_LIGHT_TRACE:
#endif
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_DRAMA;
        break;
#ifdef SAMSUNG_MAGICSHOT
    case SHOT_MODE_MAGIC:
        if (getCameraId() == CAMERA_ID_BACK) {
            mode = AA_CONTROL_USE_SCENE_MODE;
            sceneMode = AA_SCENE_MODE_DRAMA;
        } else {
            mode = AA_CONTROL_AUTO;
            sceneMode = AA_SCENE_MODE_FACE_PRIORITY;
        }
        break;
#endif
    case SHOT_MODE_3D_PANORAMA:
    case SHOT_MODE_PANORAMA:
    case SHOT_MODE_FRONT_PANORAMA:
    case SHOT_MODE_INTERACTIVE:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_PANORAMA;
        break;
    case SHOT_MODE_NIGHT:
    case SHOT_MODE_NIGHT_SCENE:
#ifdef SAMSUNG_FOOD_MODE
        if (getSceneMode() == SCENE_MODE_FOOD) {
            mode = AA_CONTROL_USE_SCENE_MODE;
            sceneMode = AA_SCENE_MODE_FOOD;
        } else
#endif
        {
            mode = AA_CONTROL_USE_SCENE_MODE;
            sceneMode = AA_SCENE_MODE_LLS;
        }
        break;
    case SHOT_MODE_ANIMATED_SCENE:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_ANIMATED;
        break;
    case SHOT_MODE_SPORTS:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_SPORTS;
        break;
    case SHOT_MODE_GOLF:
#ifdef SAMSUNG_TN_FEATURE
    case SHOT_MODE_SEQUENCE:
#endif
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_GOLF;
        break;
    case SHOT_MODE_NORMAL:
        if (getDualMode() == true) {
            mode = AA_CONTROL_USE_SCENE_MODE;
            sceneMode = AA_SCENE_MODE_DUAL;
        } else {
            mode = AA_CONTROL_AUTO;
            sceneMode = AA_SCENE_MODE_FACE_PRIORITY;
        }
        break;
    case SHOT_MODE_AUTO:
    case SHOT_MODE_BEAUTY_FACE:
    case SHOT_MODE_BEST_PHOTO:
    case SHOT_MODE_BEST_FACE:
    case SHOT_MODE_ERASER:
    case SHOT_MODE_RICH_TONE:
    case SHOT_MODE_STORY:
    case SHOT_MODE_SELFIE_ALARM:
#ifdef SAMSUNG_DOF
    case SHOT_MODE_3DTOUR:
    case SHOT_MODE_OUTFOCUS:
#endif
    case SHOT_MODE_FASTMOTION:
    case SHOT_MODE_ANTI_FOG:
        mode = AA_CONTROL_AUTO;
        sceneMode = AA_SCENE_MODE_FACE_PRIORITY;
        break;
    case SHOT_MODE_FACE_LOCK:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_FACE_LOCK;
        break;
    case SHOT_MODE_PRO_MODE:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_PRO_MODE;
        break;
    case SHOT_MODE_DUAL:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_DUAL;
        break;
    case SHOT_MODE_VIDEO_COLLAGE:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_VIDEO_COLLAGE;
        break;
#ifdef SAMSUNG_COLOR_IRIS
    case SHOT_MODE_COLOR_IRIS:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_COLOR_IRIS;
        break;
#endif
    case SHOT_MODE_AUTO_PORTRAIT:
    case SHOT_MODE_PET:
#ifdef USE_LIMITATION_FOR_THIRD_PARTY
    case THIRD_PARTY_BLACKBOX_MODE:
    case THIRD_PARTY_VTCALL_MODE:
#endif

#ifdef SAMSUNG_LIVE_OUTFOCUS
    case SHOT_MODE_LIVE_OUTFOCUS:
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_LIVE_OUTFOCUS;
        break;
#endif

    default:
        changeSceneMode = false;
        break;
    }

#ifdef LLS_CAPTURE
    if(m_flagLLSOn) {
        mode = AA_CONTROL_USE_SCENE_MODE;
        sceneMode = AA_SCENE_MODE_LLS;
    }
#endif

    m_cameraInfo.shotMode = shotMode;
    if (changeSceneMode == true)
        setMetaCtlSceneMode(&m_metadata, mode, sceneMode);
}

int ExynosCamera1Parameters::getShotMode(void)
{
    return m_cameraInfo.shotMode;
}

status_t ExynosCamera1Parameters::checkVtMode(const CameraParameters& params)
{
    int newVTMode = params.getInt("vtmode");
    int curVTMode = -1;

    CLOGD("[setParameters]newVTMode %d", newVTMode);

    /*
     * VT mode
     *   1: 3G vtmode (176x144, Fixed 7fps)
     *   2: LTE or WIFI vtmode (640x480, Fixed 15fps)
     *   3: Reserved : Smart Stay
     *   4: CHINA vtmode (1280x720, Fixed 30fps)
     */
    if (newVTMode == 3 || (newVTMode < 0 || newVTMode > 4)) {
        newVTMode = 0;
    }

    curVTMode = getVtMode();

    if (curVTMode != newVTMode) {
        m_setVtMode(newVTMode);
        m_params.set("vtmode", newVTMode);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setVtMode(int vtMode)
{
    m_cameraInfo.vtMode = vtMode;

    setMetaVtMode(&m_metadata, (enum camera_vt_mode)vtMode);
}

int ExynosCamera1Parameters::getVtMode(void)
{
    return m_cameraInfo.vtMode;
}

status_t ExynosCamera1Parameters::checkVRMode(const CameraParameters& params)
{
    int newVRMode =  params.getInt("vrmode");
    int curVRMode = getVRMode();

    CLOGD("[setParameters]newVRMode %d", newVRMode);

    if (curVRMode != newVRMode) {
        m_setVRMode(newVRMode);
        m_params.set("vrmode", newVRMode);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setVRMode(int vrMode)
{
    m_cameraInfo.vrMode = vrMode;
}

int ExynosCamera1Parameters::getVRMode(void)
{
    return m_cameraInfo.vrMode;
}

status_t ExynosCamera1Parameters::checkGamma(const CameraParameters& params)
{
    bool newGamma = false;
    bool curGamma = false;
    const char *strNewGamma = params.get("video_recording_gamma");

    if (strNewGamma == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewGamma %s", strNewGamma);

    if (!strcmp(strNewGamma, "off")) {
        newGamma = false;
    } else if (!strcmp(strNewGamma, "on")) {
        newGamma = true;
    } else {
        CLOGE("unmatched gamma(%s)", strNewGamma);
        return BAD_VALUE;
    }

    curGamma = getGamma();

    if (curGamma != newGamma) {
        m_setGamma(newGamma);
        m_params.set("video_recording_gamma", strNewGamma);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setGamma(bool gamma)
{
    m_cameraInfo.gamma = gamma;
}

bool ExynosCamera1Parameters::getGamma(void)
{
    return m_cameraInfo.gamma;
}

status_t ExynosCamera1Parameters::checkSlowAe(const CameraParameters& params)
{
    bool newSlowAe = false;
    bool curSlowAe = false;
    const char *strNewSlowAe = params.get("slow_ae");

    if (strNewSlowAe == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewSlowAe %s", strNewSlowAe);

    if (!strcmp(strNewSlowAe, "off"))
        newSlowAe = false;
    else if (!strcmp(strNewSlowAe, "on"))
        newSlowAe = true;
    else {
        CLOGE("unmatched slow_ae(%s)", strNewSlowAe);
        return BAD_VALUE;
    }

    curSlowAe = getSlowAe();

    if (curSlowAe != newSlowAe) {
        m_setSlowAe(newSlowAe);
        m_params.set("slow_ae", strNewSlowAe);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setSlowAe(bool slowAe)
{
    m_cameraInfo.slowAe = slowAe;
}

bool ExynosCamera1Parameters::getSlowAe(void)
{
    return m_cameraInfo.slowAe;
}

status_t ExynosCamera1Parameters::checkImageUniqueId(__unused const CameraParameters& params)
{
    const char *strCurImageUniqueId = m_params.get("imageuniqueid-value");
    const char *strNewImageUniqueId = NULL;

#if defined(SAMSUNG_COMPANION) || defined(SAMSUNG_EEPROM)
    if (m_romReadThreadDone != true) {
        CLOGD("[setParameters]checkImageUniqueId : romReadThreadDone(%d)", m_romReadThreadDone);
        return NO_ERROR;
    }
#endif

    if(strCurImageUniqueId == NULL || strcmp(strCurImageUniqueId, "") == 0 || strcmp(strCurImageUniqueId, "0") == 0) {
        strNewImageUniqueId = getImageUniqueId();

        if (strNewImageUniqueId != NULL && strcmp(strNewImageUniqueId, "") != 0) {
            CLOGD("[setParameters]newImageUniqueId %s ", strNewImageUniqueId );
            m_params.set("imageuniqueid-value", strNewImageUniqueId);
        }
    }
    return NO_ERROR;
}

const char *ExynosCamera1Parameters::getImageUniqueId(void)
{
#if defined(SAMSUNG_TN_FEATURE) && defined(SENSOR_FW_GET_FROM_FILE)
    char *sensorfw = NULL;
    char *uniqueid = NULL;
#ifdef FORCE_CAL_RELOAD
    char checkcal[50];

    memset(checkcal, 0, sizeof(checkcal));
#endif
    sensorfw = (char *)getSensorFWFromFile(m_staticInfo, m_cameraId);
#ifdef FORCE_CAL_RELOAD
    snprintf(checkcal, sizeof(checkcal), "%s", sensorfw);
    m_calValid = m_checkCalibrationDataValid(checkcal);
#endif

    if (getCameraId() == CAMERA_ID_BACK) {
        uniqueid = sensorfw;
    } else {
#ifdef SAMSUNG_EEPROM_FRONT
        if (SAMSUNG_EEPROM_FRONT) {
            uniqueid = sensorfw;
        } else
#endif
        {
            uniqueid = strtok(sensorfw, " ");
        }
    }
    setImageUniqueId(uniqueid);

    return (const char *)m_cameraInfo.imageUniqueId;
#else
    return m_cameraInfo.imageUniqueId;
#endif
}

#ifdef SAMSUNG_TN_FEATURE
void ExynosCamera1Parameters::setImageUniqueId(char *uniqueId)
{
    memcpy(m_cameraInfo.imageUniqueId, uniqueId, sizeof(m_cameraInfo.imageUniqueId));
}
#endif

#ifdef BURST_CAPTURE
status_t ExynosCamera1Parameters::checkSeriesShotFilePath(const CameraParameters& params)
{
    const char *seriesShotFilePath = params.get("capture-burst-filepath");

    if (seriesShotFilePath != NULL) {
        snprintf(m_seriesShotFilePath, sizeof(m_seriesShotFilePath), "%s", seriesShotFilePath);
        CLOGD("[setParameters]seriesShotFilePath %s", seriesShotFilePath);
        m_params.set("capture-burst-filepath", seriesShotFilePath);
    } else {
        CLOGD("[setParameters]seriesShotFilePath NULL");
        memset(m_seriesShotFilePath, 0, CAMERA_FILE_PATH_SIZE);
    }

    return NO_ERROR;
}

#ifdef VARIABLE_BURST_FPS
void ExynosCamera1Parameters::setBurstShotDuration(int fps)
{
    if (fps == BURSTSHOT_MIN_FPS)
        m_burstShotDuration = MIN_BURST_DURATION;
    else
        m_burstShotDuration = NORMAL_BURST_DURATION;
}

int ExynosCamera1Parameters::getBurstShotDuration(void)
{
    return m_burstShotDuration;
}
#endif /* VARIABLE_BURST_FPS */
#endif /* BURST_CAPTURE */

status_t ExynosCamera1Parameters::checkSeriesShotMode(const CameraParameters& params)
{
#ifndef SAMSUNG_TN_FEATURE
    int burstCount = params.getInt("burst-capture");
    int bestCount = params.getInt("best-capture");

    if (burstCount < 0 || bestCount < 0) {
        CLOGE(" Invalid burst-capture count(%d), best-capture count(%d)", burstCount, bestCount);
        return BAD_VALUE;
    }
    CLOGD("[setParameters]burstCount(%d), bestCount(%d)", burstCount, bestCount);

    /* TODO: select shot count */
    if (bestCount > burstCount) {
        m_setSeriesShotCount(bestCount);
        m_params.set("burst-capture", 0);
        m_params.set("best-capture", bestCount);
    } else {
        m_setSeriesShotCount(burstCount);
        m_params.set("burst-capture", burstCount);
        m_params.set("best-capture", 0);
    }
#endif
    return NO_ERROR;
}

#ifdef SAMSUNG_LLV
status_t ExynosCamera1Parameters::checkLLV(const CameraParameters& params)
{
    int mode = params.getInt("llv_mode");
    int prev_mode = m_params.getInt("llv_mode");

    if (mode !=  prev_mode) {
        CLOGW("[PARM_DBG] new LLV mode : %d ", mode);
        setLLV(mode);
        m_params.set("llv_mode", mode);
    } else {
        CLOGD("No value change in LLV mode : %d", mode);
    }

    return NO_ERROR;
}
#endif

#ifdef SAMSUNG_DOF
status_t ExynosCamera1Parameters::checkMoveLens(const CameraParameters& params)
{
    const char *newStrMoveLens = params.get("focus-bracketing-values");
    const char *curStrMoveLens = m_params.get("focus-bracketing-values");

    if(newStrMoveLens != NULL) {
        CLOGD("[checkMoveLens][DOF] string : %s", newStrMoveLens);
    }
    else {
        memset(m_cameraInfo.lensPosTbl, 0, sizeof(m_cameraInfo.lensPosTbl));
        return NO_ERROR;
    }

    if(SHOT_MODE_OUTFOCUS != getShotMode())
        return NO_ERROR;

    CLOGD("[DOF][PARM_DBG] new Lens position : %s ", newStrMoveLens);
    char *start;
    char *end;
    char delim = ',';
    int N = 6; // max count is 5 (+ number of value)

    if (newStrMoveLens != NULL) {
        start = (char *)newStrMoveLens;
        for(int i = 1; i < N; i++) {
            m_cameraInfo.lensPosTbl[i] = (int) strtol(start, &end, 10);
            CLOGD("[checkMoveLens][DOF] lensPosTbl[%d] : %d",
                    i, m_cameraInfo.lensPosTbl[i]);
            if(*end != delim) {
                m_cameraInfo.lensPosTbl[0] = i; // Total Count
                CLOGD("[checkMoveLens][DOF] lensPosTbl[0] : %d", i);
                break;
            }
            start = end+1;
        }
    }

    m_params.set("focus-bracketing-values", newStrMoveLens);

    return NO_ERROR;
}

int ExynosCamera1Parameters::getMoveLensTotal(void)
{
    return m_cameraInfo.lensPosTbl[0];
}

void ExynosCamera1Parameters::setMoveLensTotal(int count)
{
    m_cameraInfo.lensPosTbl[0] = count;
}

void ExynosCamera1Parameters::setMoveLensCount(int count)
{
    m_curLensCount = count;
    CLOGD("[setMoveLensCount][DOF] m_curLensCount : %d",
                m_curLensCount);
}

void ExynosCamera1Parameters::m_setLensPosition(int step)
{
    CLOGD("[m_setLensPosition][DOF] step : %d",
                step);
    setMetaCtlLensPos(&m_metadata, m_cameraInfo.lensPosTbl[step]);
    m_curLensStep = m_cameraInfo.lensPosTbl[step];
}
#endif

#ifdef SUPPORT_MULTI_AF
status_t ExynosCamera1Parameters::checkMultiAf(const CameraParameters& params)
{
    ExynosCameraActivityAutofocus *autoFocusMgr = m_activityControl->getAutoFocusMgr();
    bool curMultiAf = false;
    bool newMultiAf = false;
    const char *strNewMultiAf = params.get("multi-af");

    if (strNewMultiAf == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewmultiaf %s", strNewMultiAf);

    if (!strcmp(strNewMultiAf, "off")) {
        newMultiAf = false;
    } else if (!strcmp(strNewMultiAf, "on")) {
        newMultiAf = true;
    } else {
        CLOGE("Invalid MultiAf(%s)", strNewMultiAf);
        return BAD_VALUE;
    }

    curMultiAf = autoFocusMgr->getMultiAf();
    if (curMultiAf != newMultiAf) {
        autoFocusMgr->setMultiAf(newMultiAf);
        m_params.set("multi-af", strNewMultiAf);
    }
    return NO_ERROR;
}
#endif

#ifdef SAMSUNG_OIS_VDIS
void ExynosCamera1Parameters::setOISCoef(uint32_t coef)
{
    if (getZoomActiveOn()) return;

    if (m_oisFadeVal != coef) {
        m_oisFadeVal = coef;
        setMetaCtlOISCoef(&m_metadata, m_oisFadeVal);
    }
}

void ExynosCamera1Parameters::setSWVdisOISInit()
{
    m_oisFadeVal = -1;
    m_SWVdisOisGainFrameCount[0] = 0;
    m_SWVdisOisGainFrameCount[1] = 0;
}

uint32_t ExynosCamera1Parameters::getOISGain()
{
    int zoomLevel = getZoomLevel();
    int oisGain = 0;

    if (m_swVDIS_UHD) {
        oisGain = 100;
    } else {
        oisGain = (zoomLevel >= ZOOM_RATIO_3X) ? 100 : 0;
        int exposureTime = m_SWVdisPreviewFrameExposureTime;

        if (oisGain == 0) {
            int gainIdx = 0;
            if (exposureTime >= 16660000) { // 1/60s
                m_SWVdisOisGainFrameCount[0] = 0;
                gainIdx = 1;
            } else {
                m_SWVdisOisGainFrameCount[1] = 0;
                gainIdx = 0;
            }

            if (m_oisFadeVal == -1) {
                m_SWVdisOisGainFrameCount[gainIdx] = 30;
            }
            if (++m_SWVdisOisGainFrameCount[gainIdx] >= 30) {
                m_SWVdisOisGainFrameCount[gainIdx] = 30;
            }

            if (m_SWVdisOisGainFrameCount[0] == 30) {
                oisGain = 0;
            } else if (m_SWVdisOisGainFrameCount[1] == 30) {
                oisGain = 50;
            } else {
                oisGain = m_oisFadeVal;
            }
        } else {
            m_SWVdisOisGainFrameCount[0] = 30;
            m_SWVdisOisGainFrameCount[1] = 30;
        }
    }

    return oisGain;
}

void ExynosCamera1Parameters::setOISFadeVal(int oisFadeVal)
{
    m_oisFadeVal = oisFadeVal;
}

void ExynosCamera1Parameters::setSWVdisPreviewFrameExposureTime(int exposureTime)
{
    m_SWVdisPreviewFrameExposureTime = exposureTime;
}
#endif

#ifdef BURST_CAPTURE
int ExynosCamera1Parameters::getSeriesShotSaveLocation(void)
{
    int seriesShotSaveLocation = m_seriesShotSaveLocation;

#ifdef CAMERA_GED_FEATURE
#else
    int shotMode = getShotMode();
#endif

    /* GED's series shot work as callback */
#ifdef CAMERA_GED_FEATURE
    seriesShotSaveLocation = BURST_SAVE_CALLBACK;
#else
    if (shotMode == SHOT_MODE_BEST_PHOTO) {
        seriesShotSaveLocation = BURST_SAVE_CALLBACK;
#ifdef ONE_SECOND_BURST_CAPTURE
    } else if (m_cameraInfo.seriesShotMode == SERIES_SHOT_MODE_ONE_SECOND_BURST) {
        seriesShotSaveLocation = BURST_SAVE_CALLBACK;
#endif
    } else {
        if (m_seriesShotSaveLocation == 0)
            seriesShotSaveLocation = BURST_SAVE_PHONE;
        else
            seriesShotSaveLocation = BURST_SAVE_SDCARD;
    }
#endif

    return seriesShotSaveLocation;
}

void ExynosCamera1Parameters::setSeriesShotSaveLocation(int ssaveLocation)
{
    m_seriesShotSaveLocation = ssaveLocation;
}

char *ExynosCamera1Parameters::getSeriesShotFilePath(void)
{
    return m_seriesShotFilePath;
}
#endif

int ExynosCamera1Parameters::getSeriesShotDuration(void)
{
    switch (m_cameraInfo.seriesShotMode) {
    case SERIES_SHOT_MODE_BURST:
#ifdef VARIABLE_BURST_FPS
        return getBurstShotDuration();
#else
        return NORMAL_BURST_DURATION;
#endif
    case SERIES_SHOT_MODE_BEST_FACE:
        return BEST_FACE_DURATION;
    case SERIES_SHOT_MODE_BEST_PHOTO:
        return BEST_PHOTO_DURATION;
    case SERIES_SHOT_MODE_ERASER:
        return ERASER_DURATION;
#ifdef SAMSUNG_MAGICSHOT
    case SERIES_SHOT_MODE_MAGIC:
        return MAGIC_SHOT_DURATION;
#endif
    case SERIES_SHOT_MODE_SELFIE_ALARM:
        return SELFIE_ALARM_DURATION;
#ifdef ONE_SECOND_BURST_CAPTURE
    case SERIES_SHOT_MODE_ONE_SECOND_BURST:
        return ONE_SECOND_BURST_CAPTURE_DURATION;
#endif
    default:
        if (getShotMode() == SHOT_MODE_PANORAMA) {
            return PANORAMA_SHOT_DURATION;
        } else {
            return 0;
        }
    }
    return 0;
}

int ExynosCamera1Parameters::getSeriesShotMode(void)
{
    return m_cameraInfo.seriesShotMode;
}

void ExynosCamera1Parameters::setSeriesShotMode(int sshotMode, int count)
{
    int sshotCount = 0;
    int shotMode = getShotMode();
    if (sshotMode == SERIES_SHOT_MODE_BURST) {
        if (shotMode == SHOT_MODE_BEST_PHOTO) {
            sshotMode = SERIES_SHOT_MODE_BEST_PHOTO;
            sshotCount = 8;
        } else if (shotMode == SHOT_MODE_BEST_FACE) {
            sshotMode = SERIES_SHOT_MODE_BEST_FACE;
            sshotCount = 5;
        } else if (shotMode == SHOT_MODE_ERASER) {
            sshotMode = SERIES_SHOT_MODE_ERASER;
            sshotCount = 5;
        }
#ifdef SAMSUNG_MAGICSHOT
        else if (shotMode == SHOT_MODE_MAGIC) {
            sshotMode = SERIES_SHOT_MODE_MAGIC;
            if (m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1)
                sshotCount = 16;
            else
                sshotCount = 32;
        }
#endif
        else if (shotMode == SHOT_MODE_SELFIE_ALARM) {
            sshotMode = SERIES_SHOT_MODE_SELFIE_ALARM;
            sshotCount = 3;
        } else {
            sshotMode = SERIES_SHOT_MODE_BURST;
            sshotCount = MAX_SERIES_SHOT_COUNT;
        }
    } else if (sshotMode == SERIES_SHOT_MODE_LLS ||
            sshotMode == SERIES_SHOT_MODE_SIS) {
            if(count > 0) {
                sshotCount = count;
            } else {
                sshotCount = 5;
            }
#ifdef ONE_SECOND_BURST_CAPTURE
    } else if (sshotMode == SERIES_SHOT_MODE_ONE_SECOND_BURST) {
        sshotCount = ONE_SECOND_BURST_CAPTURE_COUNT;
#endif
    }

    CLOGD("set shotmode(%d), shotCount(%d)", sshotMode, sshotCount);

    m_cameraInfo.seriesShotMode = sshotMode;
    m_setSeriesShotCount(sshotCount);
}

void ExynosCamera1Parameters::m_setSeriesShotCount(int seriesShotCount)
{
    m_cameraInfo.seriesShotCount = seriesShotCount;
}

int ExynosCamera1Parameters::getSeriesShotCount(void)
{
    return m_cameraInfo.seriesShotCount;
}

void ExynosCamera1Parameters::setSamsungCamera(bool value)
{
    String8 tempStr;
    ExynosCameraActivityAutofocus *autoFocusMgr = m_activityControl->getAutoFocusMgr();

    m_cameraInfo.samsungCamera = value;
    autoFocusMgr->setSamsungCamera(value);

    /* zoom */
    if (getZoomSupported() == true) {
        int maxZoom = getMaxZoomLevel();
        CLOGI("change MaxZoomLevel and ZoomRatio List.(%d)", maxZoom);

        if (0 < maxZoom) {
            m_params.set(CameraParameters::KEY_ZOOM_SUPPORTED, "true");

            if (getSmoothZoomSupported() == true)
                m_params.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "true");
            else
                m_params.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "false");

            m_params.set(CameraParameters::KEY_MAX_ZOOM, maxZoom - 1);
            m_params.set(CameraParameters::KEY_ZOOM, ZOOM_LEVEL_0);

            int max_zoom_ratio = (int)getMaxZoomRatio();
            tempStr.setTo("");
            if (getZoomRatioList(tempStr, maxZoom, max_zoom_ratio, m_staticInfo->zoomRatioList) == NO_ERROR)
                m_params.set(CameraParameters::KEY_ZOOM_RATIOS, tempStr.c_str());
            else
                m_params.set(CameraParameters::KEY_ZOOM_RATIOS, "100");

            m_params.set("constant-growth-rate-zoom-supported", "true");

            CLOGV("[setDefaultParameters]zoomRatioList=%s", tempStr.c_str());
        } else {
            m_params.set(CameraParameters::KEY_ZOOM_SUPPORTED, "false");
            m_params.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "false");
            m_params.set(CameraParameters::KEY_MAX_ZOOM, ZOOM_LEVEL_0);
            m_params.set(CameraParameters::KEY_ZOOM, ZOOM_LEVEL_0);
        }
    } else {
        m_params.set(CameraParameters::KEY_ZOOM_SUPPORTED, "false");
        m_params.set(CameraParameters::KEY_SMOOTH_ZOOM_SUPPORTED, "false");
        m_params.set(CameraParameters::KEY_MAX_ZOOM, ZOOM_LEVEL_0);
        m_params.set(CameraParameters::KEY_ZOOM, ZOOM_LEVEL_0);
    }

#ifdef USE_FW_OPMODE
    setMetaUctlOPMode(&m_metadata, (enum camera_op_mode)value);
#endif

    /* Picture Format */
    tempStr.setTo("");
#ifdef SAMSUNG_TN_FEATURE
    tempStr = String8::format("%s,%s", CameraParameters::PIXEL_FORMAT_JPEG, CameraParameters::PIXEL_FORMAT_YUV420SP_NV21);
#else
    tempStr = String8::format("%s,%s", CameraParameters::PIXEL_FORMAT_JPEG, CameraParameters::PIXEL_FORMAT_YUV420SP);
#endif
    m_params.set(CameraParameters::KEY_SUPPORTED_PICTURE_FORMATS, tempStr);
    m_params.setPictureFormat(CameraParameters::PIXEL_FORMAT_JPEG);
}

bool ExynosCamera1Parameters::getSamsungCamera(void)
{
    return m_cameraInfo.samsungCamera;
}

void ExynosCamera1Parameters::m_initMetadata(void)
{
    memset(&m_metadata, 0x00, sizeof(struct camera2_shot_ext));
    struct camera2_shot *shot = &m_metadata.shot;

    // 1. ctl
    // request
    shot->ctl.request.id = 0;
    shot->ctl.request.metadataMode = METADATA_MODE_FULL;
    shot->ctl.request.frameCount = 0;

    // lens
    shot->ctl.lens.focusDistance = -1.0f;
    shot->ctl.lens.aperture = (float)m_staticInfo->apertureNum / (float)m_staticInfo->apertureDen;
    shot->ctl.lens.focalLength = (float)m_staticInfo->focalLengthNum / (float)m_staticInfo->focalLengthDen;

    shot->ctl.lens.filterDensity = 0.0f;
    shot->ctl.lens.opticalStabilizationMode = ::OPTICAL_STABILIZATION_MODE_OFF;

    int minFps = (m_staticInfo->minFps == 0) ? 0 : m_staticInfo->minFps;
    int maxFps = (m_staticInfo->maxFps == 0) ? 0 : m_staticInfo->maxFps;

    /* The min fps can not be '0'. Therefore it is set up default value '15'. */
    if (minFps == 0) {
        CLOGW(" Invalid min fps value(%d)", minFps);
        minFps = 15;
    }

    /*  The initial fps can not be '0' and bigger than '30'. Therefore it is set up default value '30'. */
    if (maxFps == 0 || 30 < maxFps) {
        CLOGW(" Invalid max fps value(%d)", maxFps);
        maxFps = 30;
    }

    /* sensor */
    shot->ctl.sensor.exposureTime = 0;
    shot->ctl.sensor.frameDuration = (1000 * 1000 * 1000) / maxFps;
    shot->ctl.sensor.sensitivity = 0;

    /* flash */
    shot->ctl.flash.flashMode = ::CAM2_FLASH_MODE_OFF;
    shot->ctl.flash.firingPower = 0;
    shot->ctl.flash.firingTime = 0;

    /* hotpixel */
    shot->ctl.hotpixel.mode = (enum processing_mode)0;

    /* demosaic */
    shot->ctl.demosaic.mode = (enum demosaic_processing_mode)0;

    /* noise */
    shot->ctl.noise.mode = ::PROCESSING_MODE_OFF;
    shot->ctl.noise.strength = 5;

    /* shading */
    shot->ctl.shading.mode = (enum processing_mode)0;

    /* color */
    shot->ctl.color.mode = ::COLORCORRECTION_MODE_FAST;
    static const float colorTransform[9] = {
        1.0f, 0.f, 0.f,
        0.f, 1.f, 0.f,
        0.f, 0.f, 1.f
    };

    memcpy(shot->ctl.color.transform, colorTransform, sizeof(colorTransform));

    /* tonemap */
    shot->ctl.tonemap.mode = ::TONEMAP_MODE_FAST;
    static const float tonemapCurve[4] = {
        0.f, 0.f,
        1.f, 1.f
    };

    int tonemapCurveSize = sizeof(tonemapCurve);
    int sizeOfCurve = sizeof(shot->ctl.tonemap.curveRed) / sizeof(shot->ctl.tonemap.curveRed[0]);

    for (int i = 0; i < sizeOfCurve; i += 4) {
        memcpy(&(shot->ctl.tonemap.curveRed[i]),   tonemapCurve, tonemapCurveSize);
        memcpy(&(shot->ctl.tonemap.curveGreen[i]), tonemapCurve, tonemapCurveSize);
        memcpy(&(shot->ctl.tonemap.curveBlue[i]),  tonemapCurve, tonemapCurveSize);
    }

    /* edge */
    shot->ctl.edge.mode = ::PROCESSING_MODE_OFF;
    shot->ctl.edge.strength = 5;

    /* scaler
     * Max Picture Size == Max Sensor Size - Sensor Margin
     */
    if (m_setParamCropRegion(0,
                             m_staticInfo->maxPictureW, m_staticInfo->maxPictureH,
                             m_staticInfo->maxPreviewW, m_staticInfo->maxPreviewH
                             ) != NO_ERROR) {
        CLOGE("m_setZoom() fail");
    }

    /* jpeg */
    shot->ctl.jpeg.quality = 96;
    shot->ctl.jpeg.thumbnailSize[0] = m_staticInfo->maxThumbnailW;
    shot->ctl.jpeg.thumbnailSize[1] = m_staticInfo->maxThumbnailH;
    shot->ctl.jpeg.thumbnailQuality = 100;
    shot->ctl.jpeg.gpsCoordinates[0] = 0;
    shot->ctl.jpeg.gpsCoordinates[1] = 0;
    shot->ctl.jpeg.gpsCoordinates[2] = 0;
    memset(&shot->ctl.jpeg.gpsProcessingMethod, 0x0,
        sizeof(shot->ctl.jpeg.gpsProcessingMethod));
    shot->ctl.jpeg.gpsTimestamp = 0L;
    shot->ctl.jpeg.orientation = 0L;

    /* stats */
    shot->ctl.stats.faceDetectMode = ::FACEDETECT_MODE_OFF;
    shot->ctl.stats.histogramMode = ::STATS_MODE_OFF;
    shot->ctl.stats.sharpnessMapMode = ::STATS_MODE_OFF;

    /* aa */
    shot->ctl.aa.captureIntent = ::AA_CAPTURE_INTENT_CUSTOM;
    shot->ctl.aa.mode = ::AA_CONTROL_AUTO;
    shot->ctl.aa.effectMode = ::AA_EFFECT_OFF;
    shot->ctl.aa.sceneMode = ::AA_SCENE_MODE_FACE_PRIORITY;
    shot->ctl.aa.videoStabilizationMode = (enum aa_videostabilization_mode)0;

    /* default metering is center */
    shot->ctl.aa.aeMode = ::AA_AEMODE_CENTER;
    shot->ctl.aa.aeRegions[0] = 0;
    shot->ctl.aa.aeRegions[1] = 0;
    shot->ctl.aa.aeRegions[2] = 0;
    shot->ctl.aa.aeRegions[3] = 0;
    shot->ctl.aa.aeRegions[4] = 1000;
    shot->ctl.aa.aeLock = ::AA_AE_LOCK_OFF;
#if defined(USE_SUBDIVIDED_EV)
    shot->ctl.aa.aeExpCompensation = 0; /* 21 is middle */
#else
    shot->ctl.aa.aeExpCompensation = 5; /* 5 is middle */
#endif
    shot->ctl.aa.aeTargetFpsRange[0] = minFps;
    shot->ctl.aa.aeTargetFpsRange[1] = maxFps;

    shot->ctl.aa.aeAntibandingMode = ::AA_AE_ANTIBANDING_AUTO;
    shot->ctl.aa.vendor_aeflashMode = ::AA_FLASHMODE_OFF;
    shot->ctl.aa.awbMode = ::AA_AWBMODE_WB_AUTO;
    shot->ctl.aa.awbLock = ::AA_AWB_LOCK_OFF;
    shot->ctl.aa.afMode = ::AA_AFMODE_OFF;
    shot->ctl.aa.afRegions[0] = 0;
    shot->ctl.aa.afRegions[1] = 0;
    shot->ctl.aa.afRegions[2] = 0;
    shot->ctl.aa.afRegions[3] = 0;
    shot->ctl.aa.afRegions[4] = 1000;
    shot->ctl.aa.afTrigger = (enum aa_af_trigger)0;
    shot->ctl.aa.vendor_isoMode = AA_ISOMODE_AUTO;
    shot->ctl.aa.vendor_isoValue = 0;

    /* 2. dm */

    /* 3. uctl */
#ifdef SAMSUNG_COMPANION
    shot->uctl.companionUd.caf_mode = COMPANION_CAF_ON;
    shot->uctl.companionUd.disparity_mode = COMPANION_DISPARITY_OFF;

    shot->uctl.companionUd.drc_mode = COMPANION_DRC_OFF;
    shot->uctl.companionUd.paf_mode = COMPANION_PAF_OFF;
    shot->uctl.companionUd.wdr_mode = COMPANION_WDR_OFF;
    shot->uctl.companionUd.lsc_mode = COMPANION_LSC_OFF;
    shot->uctl.companionUd.bpc_mode = COMPANION_BPC_OFF;
    shot->uctl.companionUd.bypass_mode= COMPANION_FULL_BYPASS_OFF;
#endif
#ifdef USE_FW_ZOOMRATIO
    shot->uctl.zoomRatio = 1.00f;
#else
    shot->uctl.zoomRatio = 0;
#endif

    /* 4. udm */

    /* 5. magicNumber */
    shot->magicNumber = SHOT_MAGIC_NUMBER;

    setMetaSetfile(&m_metadata, 0x0);

    /* user request */
    m_metadata.drc_bypass = 1;
    m_metadata.dis_bypass = 1;
    m_metadata.dnr_bypass = 1;
    m_metadata.fd_bypass  = 1;
}

status_t ExynosCamera1Parameters::duplicateCtrlMetadata(void *buf)
{
    if (buf == NULL) {
        CLOGE("ERR: buf is NULL");
        return BAD_VALUE;
    }

    struct camera2_shot_ext *meta_shot_ext = (struct camera2_shot_ext *)buf;
    memcpy(&meta_shot_ext->shot.ctl, &m_metadata.shot.ctl, sizeof(struct camera2_ctl));

#ifdef SAMSUNG_COMPANION
    memcpy(&meta_shot_ext->shot.uctl.companionUd,
                  &m_metadata.shot.uctl.companionUd,
                  sizeof(struct camera2_companion_uctl));
#endif

#ifdef SUPPORT_DEPTH_MAP
    meta_shot_ext->shot.uctl.companionUd.disparity_mode = m_metadata.shot.uctl.companionUd.disparity_mode;
#endif

#ifdef SAMSUNG_DOF
    if(getShotMode() == SHOT_MODE_OUTFOCUS && m_curLensCount > 0) {
        meta_shot_ext->shot.uctl.lensUd.pos = m_metadata.shot.uctl.lensUd.pos;
        meta_shot_ext->shot.uctl.lensUd.posSize = m_metadata.shot.uctl.lensUd.posSize;
        meta_shot_ext->shot.uctl.lensUd.direction = m_metadata.shot.uctl.lensUd.direction;
        meta_shot_ext->shot.uctl.lensUd.slewRate = m_metadata.shot.uctl.lensUd.slewRate;
    }
#endif

#ifdef SAMSUNG_HRM
    if ((getCameraId() == CAMERA_ID_BACK || getCameraId() == CAMERA_ID_BACK_1)
            && m_getHRMHint() == true) {
#ifdef SAMSUNG_DUAL_SOLUTION
        if (getDualCameraMode() == true) {
            if (m_dualParameters->getDualMetadata()->shot.uctl.aaUd.hrmInfo.ir_data != 0)
                meta_shot_ext->shot.uctl.aaUd.hrmInfo =
                        m_dualParameters->getDualMetadata()->shot.uctl.aaUd.hrmInfo;
        } else
#endif
        if (m_metadata.shot.uctl.aaUd.hrmInfo.ir_data != 0)
            meta_shot_ext->shot.uctl.aaUd.hrmInfo = m_metadata.shot.uctl.aaUd.hrmInfo;
    }
#endif
#ifdef SAMSUNG_LIGHT_IR
    if ((getCameraId() == CAMERA_ID_FRONT || getCameraId() == CAMERA_ID_FRONT_1) && (m_metadata.shot.uctl.aaUd.illuminationInfo.visible_exptime != 0)
                    && m_flagSensorLight_IR_Hint) {
        meta_shot_ext->shot.uctl.aaUd.illuminationInfo = m_metadata.shot.uctl.aaUd.illuminationInfo;
    }
#endif
#ifdef SAMSUNG_GYRO
#ifndef SAMSUNG_GYRO_FRONT
    if (getCameraId() == CAMERA_ID_BACK || getCameraId() == CAMERA_ID_BACK_1)
#endif
    {
        if (m_getGyroHint()) {
#ifdef SAMSUNG_DUAL_SOLUTION
            if (getDualCameraMode() == true) {
                meta_shot_ext->shot.uctl.aaUd.gyroInfo =
                        m_dualParameters->getDualMetadata()->shot.uctl.aaUd.gyroInfo;
            } else
#endif
            meta_shot_ext->shot.uctl.aaUd.gyroInfo = m_metadata.shot.uctl.aaUd.gyroInfo;
        }
    }
#endif
#ifdef SAMSUNG_ACCELEROMETER
    if ((getCameraId() == CAMERA_ID_BACK || getCameraId() == CAMERA_ID_BACK_1)
            && m_getAccelerometerHint() == true) {
#ifdef SAMSUNG_DUAL_SOLUTION
        if (getDualCameraMode() == true) {
            meta_shot_ext->shot.uctl.aaUd.accInfo =
                    m_dualParameters->getDualMetadata()->shot.uctl.aaUd.accInfo;
        } else
#endif
        meta_shot_ext->shot.uctl.aaUd.accInfo = m_metadata.shot.uctl.aaUd.accInfo;
    }
#endif
#ifdef USE_FW_ZOOMRATIO
#ifdef USE_DUAL_CAMERA
    meta_shot_ext->shot.uctl.zoomRatio = m_metadata.shot.uctl.zoomRatio;
#else
    if(getCameraId() == CAMERA_ID_BACK) {
        meta_shot_ext->shot.uctl.zoomRatio = m_metadata.shot.uctl.zoomRatio;
    }
#endif // USE_DUAL_CAMERA
    meta_shot_ext->shot.uctl.statsRoi[0] = m_metadata.shot.uctl.statsRoi[0];
    meta_shot_ext->shot.uctl.statsRoi[1] = m_metadata.shot.uctl.statsRoi[1];
    meta_shot_ext->shot.uctl.statsRoi[2] = m_metadata.shot.uctl.statsRoi[2];
    meta_shot_ext->shot.uctl.statsRoi[3] = m_metadata.shot.uctl.statsRoi[3];
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true) {
        if (getCameraId() == CAMERA_ID_BACK_1)
            meta_shot_ext->shot.uctl.masterCam = (aa_cameratype)getMasterCam();
        else
            meta_shot_ext->shot.uctl.masterCam = m_metadata.shot.uctl.masterCam;
    } else {
        meta_shot_ext->shot.uctl.masterCam = m_metadata.shot.uctl.masterCam;
    }
#else
    meta_shot_ext->shot.uctl.masterCam = m_metadata.shot.uctl.masterCam;
#endif
#endif // USE_FW_ZOOMRATIO
#ifdef SAMSUNG_OIS_VDIS
    if(getCameraId() == CAMERA_ID_BACK) {
        meta_shot_ext->shot.uctl.lensUd.oisCoefVal = m_metadata.shot.uctl.lensUd.oisCoefVal;
    }
#endif
    meta_shot_ext->shot.uctl.vtMode = m_metadata.shot.uctl.vtMode;

#ifdef USE_FW_ZOOMRATIO
#ifdef USE_DUAL_CAMERA
    meta_shot_ext->shot.uctl.zoomRatio = m_metadata.shot.uctl.zoomRatio;
#else
    if(getCameraId() == CAMERA_ID_BACK) {
        meta_shot_ext->shot.uctl.zoomRatio = m_metadata.shot.uctl.zoomRatio;
    }
#endif // USE_DUAL_CAMERA
#endif // USE_FW_ZOOMRATIO

#ifdef USE_FW_FLASHMODE
    meta_shot_ext->shot.uctl.flashMode = m_metadata.shot.uctl.flashMode;
#endif
#ifdef USE_FW_OPMODE
    meta_shot_ext->shot.uctl.opMode = m_metadata.shot.uctl.opMode;
#endif

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setDualShotMode(const CameraParameters& params)
{
    const char *strParamSceneMode = params.get(CameraParameters::KEY_SCENE_MODE);
    int newShotMode = params.getInt("shot-mode");

    if (strParamSceneMode == NULL) {
        CLOGD("strParamSceneMode is NULL.");
        return;
    }

    if (!strcmp(strParamSceneMode, CameraParameters::SCENE_MODE_AUTO) &&
        newShotMode ==  SHOT_MODE_NORMAL) {
        enum aa_mode mode = AA_CONTROL_USE_SCENE_MODE;
        enum aa_scene_mode sceneMode = AA_SCENE_MODE_DUAL;

        setMetaCtlSceneMode(&m_metadata, mode, sceneMode);
    } else {
        CLOGW("must to check shot mode(%d) and scene modee(%s).",
             newShotMode, strParamSceneMode);
    }
}

#ifdef LLS_CAPTURE
int ExynosCamera1Parameters::getLLS(struct camera2_shot_ext *shot)
{
    int ret = LLS_LEVEL_ZSL;

#ifdef OIS_CAPTURE
    m_llsValue = shot->shot.dm.stats.vendor_LowLightMode;
#endif

#ifdef RAWDUMP_CAPTURE
    ret = LLS_LEVEL_ZSL;
    return ret;
#endif

#if (LLS_VALUE_VERSION == 5)
    ret = shot->shot.dm.stats.vendor_LowLightMode;

    CLOGV("m_flagLLSOn(%d) getFlashMode(%d), LowLightMode(%d)",
         m_flagLLSOn, getFlashMode(), shot->shot.dm.stats.vendor_LowLightMode);

#elif (LLS_VALUE_VERSION == 4)
    ret = shot->shot.dm.stats.vendor_LowLightMode;

    if (m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1) {
        if (isLDCapture(m_cameraId)) {
            if (m_flagLLSOn) {
#ifdef SAMSUNG_FRONT_LCD_FLASH
                switch (getFlashMode()) {
                    case FLASH_MODE_OFF:
                        if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH) {
                            ret = LLS_LEVEL_MULTI_MERGE_INDICATOR_4;
                        } else {
                            ret = shot->shot.dm.stats.vendor_LowLightMode;
                        }
                        break;
                    case FLASH_MODE_ON:
                        ret = LLS_LEVEL_HIGH;
                        break;
                    case FLASH_MODE_AUTO:
                    default:
                        ret = shot->shot.dm.stats.vendor_LowLightMode;
                        break;
                }
#else
                ret = shot->shot.dm.stats.vendor_LowLightMode;
#endif
            } else {
                if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_ZSL_LIKE1) {
                    ret = LLS_LEVEL_ZSL;
                } else {
                    ret = shot->shot.dm.stats.vendor_LowLightMode;
                }
            }
        }
        return ret;
    }

    if (m_flagLLSOn) {
        switch (getFlashMode()) {
            case FLASH_MODE_AUTO:
                if ((m_flagLDCaptureMode >= STATE_LLS_LEVEL_FLASH_2
                        && m_flagLDCaptureMode <= STATE_LLS_LEVEL_FLASH_4)
                    || (m_flagLDCaptureMode >= STATE_LLS_LEVEL_FLASH_LOW_2
                        && m_flagLDCaptureMode <= STATE_LLS_LEVEL_FLASH_LOW_4)) {
                    ret = LLS_LEVEL_HIGH;
                } else {
                    ret = shot->shot.dm.stats.vendor_LowLightMode;
                }
                break;
            case FLASH_MODE_OFF:
                if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_FLASH_2) {
                    ret = LLS_LEVEL_MULTI_MERGE_INDICATOR_2;
                } else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_FLASH_3) {
                    ret = LLS_LEVEL_MULTI_MERGE_INDICATOR_3;
                } else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_FLASH_4) {
                    ret = LLS_LEVEL_MULTI_MERGE_INDICATOR_4;
                } else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH) {
                    ret = LLS_LEVEL_ZSL_LIKE1;
                } else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_FLASH_LOW_2) {
                    ret = LLS_LEVEL_MULTI_MERGE_INDICATOR_LOW_2;
                } else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_FLASH_LOW_3) {
                    ret = LLS_LEVEL_MULTI_MERGE_INDICATOR_LOW_3;
                } else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_FLASH_LOW_4) {
                    ret = LLS_LEVEL_MULTI_MERGE_INDICATOR_LOW_4;
                } else {
                    ret = shot->shot.dm.stats.vendor_LowLightMode;
                }
                break;
            case FLASH_MODE_ON:
            case FLASH_MODE_TORCH:
            case FLASH_MODE_RED_EYE:
                if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_MANUAL_ISO
                    || shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_SHARPEN_SINGLE) {
                    ret = shot->shot.dm.stats.vendor_LowLightMode;
                } else {
#ifdef FLASHED_LLS_CAPTURE
                    if (getFlashedLLSCapture()) {
                        if(shot->shot.dm.stats.vendor_LowLightMode > LLS_LEVEL_ZSL) {
                            ret = LLS_LEVEL_ZSL_LIKE1;
                        } else {
                            ret = shot->shot.dm.stats.vendor_LowLightMode;
                        }
                    } else
#endif
                    {
                        ret = LLS_LEVEL_HIGH;
                    }
                }
                break;
            default:
                ret = shot->shot.dm.stats.vendor_LowLightMode;
                break;
        }
    } else {
        switch (getFlashMode()) {
            case FLASH_MODE_AUTO:
                ret = shot->shot.dm.stats.vendor_LowLightMode;
                break;
            case FLASH_MODE_OFF:
                if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH)
                    ret = LLS_LEVEL_ZSL_LIKE1;
                else
                    ret = shot->shot.dm.stats.vendor_LowLightMode;
                break;
            case FLASH_MODE_ON:
            case FLASH_MODE_TORCH:
            case FLASH_MODE_RED_EYE:
                if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_MANUAL_ISO
                    || shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_SHARPEN_SINGLE)
                    ret = shot->shot.dm.stats.vendor_LowLightMode;
                else
                    ret = LLS_LEVEL_HIGH;
                break;
            default:
                ret = shot->shot.dm.stats.vendor_LowLightMode;
                break;
        }
    }

    CLOGV("v4 m_flagLLSOn(%d) getFlashMode(%d), LowLightMode(%d)",
             m_flagLLSOn, getFlashMode(), shot->shot.dm.stats.vendor_LowLightMode);

#elif (LLS_VALUE_VERSION == 3)
    ret = shot->shot.dm.stats.vendor_LowLightMode;

    if (m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1) {
        return shot->shot.dm.stats.vendor_LowLightMode;
    }

    if (m_flagLLSOn) {
        switch (getFlashMode()) {
        case FLASH_MODE_AUTO:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_OFF:
            if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH)
                ret = LLS_LEVEL_LOW;
            else
                ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_ON:
        case FLASH_MODE_TORCH:
        case FLASH_MODE_RED_EYE:
            ret = LLS_LEVEL_HIGH;
            break;
        default:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        }
    } else {
        switch (getFlashMode()) {
        case FLASH_MODE_AUTO:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_OFF:
            if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH)
                ret = LLS_LEVEL_ZSL_LIKE;
            else
                ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_ON:
        case FLASH_MODE_TORCH:
        case FLASH_MODE_RED_EYE:
            ret = LLS_LEVEL_HIGH;
            break;
        default:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        }
    }

    CLOGV("m_flagLLSOn(%d) getFlashMode(%d), LowLightMode(%d)",
         m_flagLLSOn, getFlashMode(), shot->shot.dm.stats.vendor_LowLightMode);

#elif (LLS_VALUE_VERSION == 2)
    ret = shot->shot.dm.stats.vendor_LowLightMode;

    if (m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1) {
        return shot->shot.dm.stats.vendor_LowLightMode;
    }

    if (m_flagLLSOn) {
        switch (getFlashMode()) {
        case FLASH_MODE_AUTO:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_OFF:
            if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_FLASH)
                ret = LLS_LEVEL_LOW;
            else
                ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_ON:
        case FLASH_MODE_TORCH:
        case FLASH_MODE_RED_EYE:
            ret = LLS_LEVEL_HIGH;
            break;
        default:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        }
    } else {
        switch (getFlashMode()) {
        case FLASH_MODE_AUTO:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_OFF:
            if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH)
                ret = LLS_LEVEL_ZSL_LIKE;
            else
                ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        case FLASH_MODE_ON:
        case FLASH_MODE_TORCH:
        case FLASH_MODE_RED_EYE:
            ret = LLS_LEVEL_HIGH;
            break;
        default:
            ret = shot->shot.dm.stats.vendor_LowLightMode;
            break;
        }
    }

    CLOGV("m_flagLLSOn(%d) getFlashMode(%d), LowLightMode(%d)",
         m_flagLLSOn, getFlashMode(), shot->shot.dm.stats.vendor_LowLightMode);

#else /* not defined FLASHED_LLS_CAPTURE */
    ret = LLS_LEVEL_ZSL;

    switch (getFlashMode()) {
    case FLASH_MODE_OFF:
        if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_LOW
            || shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH)
            ret = LLS_LEVEL_LOW;
        else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_SIS)
            ret = LLS_LEVEL_SIS;
        break;
    case FLASH_MODE_AUTO:
        if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_LOW)
            ret = LLS_LEVEL_LOW;
        else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_HIGH)
            ret = LLS_LEVEL_ZSL;
        else if (shot->shot.dm.stats.vendor_LowLightMode == STATE_LLS_LEVEL_SIS)
            ret = LLS_LEVEL_SIS;
        break;
    case FLASH_MODE_ON:
    case FLASH_MODE_TORCH:
    case FLASH_MODE_RED_EYE:
    default:
        ret = LLS_LEVEL_ZSL;
        break;
    }

#if defined(LLS_NOT_USE_SIS_FRONT)
    if ((getCameraId() == CAMERA_ID_FRONT || getCameraId() == CAMERA_ID_FRONT_1) && (ret == LLS_LEVEL_SIS)) {
        ret = LLS_LEVEL_ZSL;
    }
#endif

    CLOGV("m_flagLLSOn(%d) getFlashMode(%d), LowLightMode(%d), shotmode(%d)",
         m_flagLLSOn, getFlashMode(), shot->shot.dm.stats.vendor_LowLightMode,getShotMode());

#endif

    return ret;
}


void ExynosCamera1Parameters::setLLSOn(uint32_t enable)
{
    m_flagLLSOn = enable;
}

bool ExynosCamera1Parameters::getLLSOn(void)
{
    return m_flagLLSOn;
}

void ExynosCamera1Parameters::setLLSValue(int value)
{
#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true) {
        int masterCameraId = -1;
        int slaveCameraId = -1;

        getDualCameraId(&masterCameraId, &slaveCameraId);
        if (m_cameraId == masterCameraId) {
            if (getDualCameraSyncType() != SYNC_TYPE_SWITCH)
                m_dualParameters->setLLSValue(m_cameraId, value);
        } else {
            if (getDualCameraSyncType() == SYNC_TYPE_SWITCH)
                m_dualParameters->setLLSValue(m_cameraId, value);
        }
    }
#endif
    m_LLSValue = value;
}

int ExynosCamera1Parameters::getLLSValue(void)
{
#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true)
        return m_dualParameters->getLLSValue(m_cameraId);
    else
#endif
        return m_LLSValue;
}

void ExynosCamera1Parameters::m_setLLSShotMode(void)
{
    const char *strCurParamSceneMode = m_params.get(CameraParameters::KEY_SCENE_MODE);

    if (strCurParamSceneMode == NULL) {
        CLOGD("strCurParamSceneMode is NULL.");
        return;
    }

    if (!strcmp(strCurParamSceneMode, CameraParameters::SCENE_MODE_AUTO)) {
        enum aa_mode mode = AA_CONTROL_USE_SCENE_MODE;
        enum aa_scene_mode sceneMode = AA_SCENE_MODE_LLS;

        setMetaCtlSceneMode(&m_metadata, mode, sceneMode);
    } else {
        CLOGW("CurSceneMode is not Auto Mode.(%s)",
             strCurParamSceneMode);
    }
}

void ExynosCamera1Parameters::m_setLLSShotMode(const CameraParameters& params)
{
    const char *strParamSceneMode = params.get(CameraParameters::KEY_SCENE_MODE);

    if (strParamSceneMode == NULL) {
        CLOGD("strNewSceneMode is NULL.");
        return;
    }

    if (!strcmp(strParamSceneMode, CameraParameters::SCENE_MODE_AUTO)) {
        enum aa_mode mode = AA_CONTROL_USE_SCENE_MODE;
        enum aa_scene_mode sceneMode = AA_SCENE_MODE_LLS;

        setMetaCtlSceneMode(&m_metadata, mode, sceneMode);
    } else {
        CLOGE("SceneMode is not Auto Mode.(%s)",
             strParamSceneMode);
    }
}

#ifdef SET_LLS_CAPTURE_SETFILE
void ExynosCamera1Parameters::setLLSCaptureOn(bool enable)
{
    m_LLSCaptureOn = enable;
}

int ExynosCamera1Parameters::getLLSCaptureOn()
{
    return m_LLSCaptureOn;
}
#endif

#ifdef LLS_REPROCESSING
void ExynosCamera1Parameters::setLLSCaptureCount(int count)
{
    m_LLSCaptureCount = count;
}

int ExynosCamera1Parameters::getLLSCaptureCount()
{
    return m_LLSCaptureCount;
}
#endif
#endif

#ifdef SR_CAPTURE
void ExynosCamera1Parameters::setSROn(uint32_t enable)
{
    m_flagSRSOn = (enable > 0) ? true : false;
    setNoBayerCrop(m_flagSRSOn);
}

bool ExynosCamera1Parameters::getSROn(void)
{
    return m_flagSRSOn;
}
#endif

#if defined(SR_CAPTURE) || defined(SAMSUNG_HIFI_LLS)
void ExynosCamera1Parameters::setNoBayerCrop(bool enable)
{
    m_flagNoBayerCrop = enable;
}
bool ExynosCamera1Parameters::getNoBayerCrop(void)
{
    return m_flagNoBayerCrop;
}
#endif

#ifdef OIS_CAPTURE
void ExynosCamera1Parameters::checkOISCaptureMode(void)
{
    ExynosCameraActivityFlash *m_flashMgr = m_activityControl->getFlashMgr();
    uint32_t lls = getLLSValue();

#ifdef FLASHED_LLS_CAPTURE
    setFlashedLLSCapture(false);
#endif

    if (getCaptureExposureTime() > 0) {
        CLOGD("zsl-like capture mode off for manual shutter speed");
        setOISCaptureModeOn(false);
        return;
    }

    if ((getRecordingHint() == true) || (getShotMode() > SHOT_MODE_AUTO && getShotMode() != SHOT_MODE_NIGHT)) {
        if (getRecordingHint() == true
            || (getShotMode() != SHOT_MODE_BEAUTY_FACE
                && getShotMode() != SHOT_MODE_OUTFOCUS
                && getShotMode() != SHOT_MODE_SELFIE_ALARM
                && getShotMode() != SHOT_MODE_PRO_MODE)) {
            setOISCaptureModeOn(false);
            CLOGD("zsl-like capture mode off for special shot");
            return;
        }
    }

#ifdef SR_CAPTURE
    if (getSROn() == true) {
        CLOGD("zsl-like multi capture mode off (SR mode)");
        setOISCaptureModeOn(false);
        return;
    }
#endif

#ifdef FLASHED_LLS_CAPTURE
    if (m_flashMgr->getNeedCaptureFlash() && lls == LLS_LEVEL_FLASH) {
        CLOGD("zsl-like flash lls capture mode on, lls(%d)", m_llsValue);
        setFlashedLLSCapture(true);
        setOISCaptureModeOn(true);
        return;
    }
#endif

    if (getSeriesShotMode() == SERIES_SHOT_MODE_NONE && m_flashMgr->getNeedCaptureFlash()
#ifdef FLASHED_LLS_CAPTURE
        && !getFlashedLLSCapture()
#endif
        ) {
        setOISCaptureModeOn(false);
        CLOGD("zsl-like capture mode off for flash single capture");
        return;
    }

    if (getSeriesShotCount() > 0 && m_llsValue > 0
#ifdef SUPPORT_ZSL_MULTIFRAME
        && (!((m_cameraId == CAMERA_ID_BACK || m_cameraId == CAMERA_ID_BACK_1)
            && lls >= LLS_LEVEL_MULTI_MERGE_ZSL_2 && lls <= LLS_LEVEL_MULTI_MERGE_ZSL_4))
#endif
        ) {
        CLOGD("zsl-like multi capture mode on, lls(%d)", m_llsValue);
        setOISCaptureModeOn(true);
        return;
    }

    CLOGD("Low Light value(%d), m_flagLLSOn(%d),getFlashMode(%d)",
             lls, m_flagLLSOn, getFlashMode());

    if (m_flagLLSOn) {
        switch (getFlashMode()) {
        case FLASH_MODE_AUTO:
        case FLASH_MODE_OFF:
            if (lls == LLS_LEVEL_ZSL_LIKE || lls == LLS_LEVEL_LOW || lls == LLS_LEVEL_FLASH
                    || lls == LLS_LEVEL_MANUAL_ISO
#ifdef SAMSUNG_LLS_DEBLUR
                    || lls == LLS_LEVEL_ZSL_LIKE1
                    || (lls >= LLS_LEVEL_MULTI_MERGE_2 && lls <= LLS_LEVEL_MULTI_MERGE_INDICATOR_LOW_4)
#ifdef SUPPORT_ZSL_MULTIFRAME
                    || (lls >= LLS_LEVEL_MULTI_MERGE_LOW_2 && lls <= LLS_LEVEL_MULTI_MERGE_LOW_4)
#endif
                    || lls == LLS_LEVEL_DUMMY
#endif
               ) {
                setOISCaptureModeOn(true);
            }
            break;
        case FLASH_MODE_ON:
        case FLASH_MODE_TORCH:
        case FLASH_MODE_RED_EYE:
            if (lls == LLS_LEVEL_FLASH) {
                setOISCaptureModeOn(true);
            }
            break;
        default:
            setOISCaptureModeOn(false);
            break;
        }
    } else {
        switch (getFlashMode()) {
        case FLASH_MODE_AUTO:
            if ((lls == LLS_LEVEL_ZSL_LIKE || lls == LLS_LEVEL_ZSL_LIKE1 || lls == LLS_LEVEL_MANUAL_ISO)
                && m_flashMgr->getNeedFlash() == false) {
                setOISCaptureModeOn(true);
            }
            break;
        case FLASH_MODE_OFF:
            if (lls == LLS_LEVEL_ZSL_LIKE || lls == LLS_LEVEL_ZSL_LIKE1 || lls == LLS_LEVEL_MANUAL_ISO) {
                setOISCaptureModeOn(true);
            }
            break;
        default:
            setOISCaptureModeOn(false);
            break;
        }
    }
}

void ExynosCamera1Parameters::setOISCaptureModeOn(bool enable)
{
    m_flagOISCaptureOn = enable;
}

bool ExynosCamera1Parameters::getOISCaptureModeOn(void)
{
    return m_flagOISCaptureOn;
}
#endif

void ExynosCamera1Parameters::setMotionPhotoOn(bool enable)
{
    m_flagMotionPhotoOn = enable;
}

bool ExynosCamera1Parameters::getMotionPhotoOn()
{
    return m_flagMotionPhotoOn;
}

#ifdef RAWDUMP_CAPTURE
void ExynosCamera1Parameters::setRawCaptureModeOn(bool enable)
{
    m_flagRawCaptureOn = enable;
}

bool ExynosCamera1Parameters::getRawCaptureModeOn(void)
{
    return m_flagRawCaptureOn;
}
#endif

#ifdef SAMSUNG_DNG
void ExynosCamera1Parameters::setDNGCaptureModeOn(bool enable)
{
    m_flagDNGCaptureOn = enable;
}

bool ExynosCamera1Parameters::getDNGCaptureModeOn(void)
{
    return m_flagDNGCaptureOn;
}

void ExynosCamera1Parameters::setUseDNGCapture(bool enable)
{
    m_use_DNGCapture = enable;
}

bool ExynosCamera1Parameters::getUseDNGCapture(void)
{
    return m_use_DNGCapture;
}

void ExynosCamera1Parameters::setCheckMultiFrame(bool enable)
{
    m_flagMultiFrame = enable;
}

bool ExynosCamera1Parameters::getCheckMultiFrame(void)
{
    return m_flagMultiFrame;
}
#endif

#ifdef SAMSUNG_LLS_DEBLUR
void ExynosCamera1Parameters::checkLDCaptureMode(void)
{
    ExynosCameraActivityFlash *m_flashMgr = m_activityControl->getFlashMgr();
    setLDCaptureMode(MULTI_SHOT_MODE_NONE);
    m_LDCaptureCount = 0;
    uint32_t lls = getLLSValue();
    int zoomLevel = getZoomLevel();
    float zoomRatio = getZoomRatio(zoomLevel) / 1000;

#ifdef SUPPORT_ZSL_MULTIFRAME
    checkZslMultiframeMode();
#endif

    if ((m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1)
        && (getRecordingHint() == true
            || getDualMode() == true
            || (getShotMode() > SHOT_MODE_AUTO && getShotMode() != SHOT_MODE_NIGHT))) {
        CLOGD("LLS Deblur capture mode off for special shot");
        setLDCaptureMode(MULTI_SHOT_MODE_NONE);
        setLDCaptureLLSValue(LLS_LEVEL_ZSL);
        return;
    }

#ifdef SAMSUNG_DUAL_CAPTURE_SOLUTION
    if (getDualCameraMode() == true &&
        getDualCameraReprocessingSyncType() == SYNC_TYPE_SYNC &&
        getFusionCaptureMode(m_cameraId, false) == true) {
        CLOGD("LLS Deblur capture mode off for dual capture fusion");
        setLDCaptureMode(MULTI_SHOT_MODE_NONE);
        setLDCaptureLLSValue(LLS_LEVEL_ZSL);
        return;
    }
#endif

#if 0 /* REMOVE - IQ TEAM GUIDE */
    if (m_cameraId == CAMERA_ID_BACK && zoomRatio >= 3.0f) {
        CLOGD("LLS Deblur capture mode off. zoomRatio(%f) LLS Value(%d)",
                 zoomRatio, getLLSValue());
        setLDCaptureMode(MULTI_SHOT_MODE_NONE);
        setLDCaptureCount(0);
        setLDCaptureLLSValue(LLS_LEVEL_ZSL);
        return;
    }
#endif

    if (m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1
#ifdef FLASHED_LLS_CAPTURE
        || getFlashedLLSCapture() == true
#endif
#ifdef SUPPORT_ZSL_MULTIFRAME
        || getZslMultiframeModeOn() == true
#endif
        || getOISCaptureModeOn() == true) {
        if (lls == LLS_LEVEL_MULTI_MERGE_2
            || lls == LLS_LEVEL_MULTI_PICK_2
#ifdef SUPPORT_ZSL_MULTIFRAME
            || lls == LLS_LEVEL_MULTI_MERGE_LOW_2
            || lls == LLS_LEVEL_MULTI_MERGE_ZSL_2
#endif
            || lls == LLS_LEVEL_MULTI_MERGE_INDICATOR_2
            || lls == LLS_LEVEL_MULTI_MERGE_INDICATOR_LOW_2) {
            setLDCaptureMode(MULTI_SHOT_MODE_MULTI1);
            setLDCaptureCount(LD_CAPTURE_COUNT_MULTI1);
            setOutPutFormatNV21Enable(true);
        } else if (lls == LLS_LEVEL_MULTI_MERGE_3
#ifdef SUPPORT_ZSL_MULTIFRAME
                   || lls == LLS_LEVEL_MULTI_MERGE_LOW_3
                   || lls == LLS_LEVEL_MULTI_MERGE_ZSL_3
#endif
                   || lls == LLS_LEVEL_MULTI_PICK_3
                   || lls == LLS_LEVEL_MULTI_MERGE_INDICATOR_3
                   || lls == LLS_LEVEL_MULTI_MERGE_INDICATOR_LOW_3) {
            setLDCaptureMode(MULTI_SHOT_MODE_MULTI2);
            setLDCaptureCount(LD_CAPTURE_COUNT_MULTI2);
            setOutPutFormatNV21Enable(true);
        } else if (lls == LLS_LEVEL_MULTI_MERGE_4
                   || lls == LLS_LEVEL_MULTI_PICK_4
#ifdef SUPPORT_ZSL_MULTIFRAME
                   || lls == LLS_LEVEL_MULTI_MERGE_LOW_4
                   || lls == LLS_LEVEL_MULTI_MERGE_ZSL_4
#endif
                   || lls == LLS_LEVEL_MULTI_MERGE_INDICATOR_4
                   || lls == LLS_LEVEL_MULTI_MERGE_INDICATOR_LOW_4) {
            setLDCaptureMode(MULTI_SHOT_MODE_MULTI3);
            setLDCaptureCount(LD_CAPTURE_COUNT_MULTI3);
            setOutPutFormatNV21Enable(true);
        }
#ifdef FLASHED_LLS_CAPTURE
        else if (getFlashedLLSCapture()) {
            setLDCaptureMode(MULTI_SHOT_MODE_FLASHED_LLS);
            setLDCaptureCount(FLASHED_LLS_CAPTURE_COUNT);
            setOutPutFormatNV21Enable(true);
        }
#endif
        else {
            setLDCaptureMode(MULTI_SHOT_MODE_NONE);
            setLDCaptureCount(0);
        }
    }

    setLDCaptureLLSValue(lls);

#ifdef SET_LLS_CAPTURE_SETFILE
    if (getLDCaptureMode() > MULTI_SHOT_MODE_NONE
        && getLDCaptureMode() <= MULTI_SHOT_MODE_MULTI3) {
        CLOGD("[LLS_MBR]set LLS Capture mode on. lls value(%d) LDCaptureLLSValue(%d) OISCaptureModeOn(%d)", getLLSValue(), getLDCaptureLLSValue(), getOISCaptureModeOn());
        setLLSCaptureOn(true);
    }
#endif
}

void ExynosCamera1Parameters::setLDCaptureMode(int LDMode)
{
#ifdef SAMSUNG_HIFI_LLS
    if (LDMode == MULTI_SHOT_MODE_NONE)
        setNoBayerCrop(false);
    else
        setNoBayerCrop(true);
#endif
    m_flagLDCaptureMode = LDMode;
}

int ExynosCamera1Parameters::getLDCaptureMode(void)
{
    return m_flagLDCaptureMode;
}

void ExynosCamera1Parameters::setLDCaptureLLSValue(uint32_t lls)
{
    m_flagLDCaptureLLSValue = lls;
}

int ExynosCamera1Parameters::getLDCaptureLLSValue(void)
{
    return m_flagLDCaptureLLSValue;
}

void ExynosCamera1Parameters::setLDCaptureCount(int count)
{
    m_LDCaptureCount = count;
}

int ExynosCamera1Parameters::getLDCaptureCount(void)
{
    return m_LDCaptureCount;
}

#ifdef SUPPORT_ZSL_MULTIFRAME
void ExynosCamera1Parameters::checkZslMultiframeMode(void)
{
    ExynosCameraActivityFlash *m_flashMgr = m_activityControl->getFlashMgr();
    uint32_t lls = getLLSValue();

    setZslMultiframeModeOn(false);

    if (!(m_cameraId == CAMERA_ID_BACK || m_cameraId == CAMERA_ID_BACK_1))
        return;

    if (!(lls >= LLS_LEVEL_MULTI_MERGE_ZSL_2
        && lls <= LLS_LEVEL_MULTI_MERGE_ZSL_4))
        return;

    if (getCaptureExposureTime() > 0) {
        CLOGD("zsl multi-frame mode off for manual shutter speed");
        return;
    }

    if ((getRecordingHint() == true) || (getShotMode() > SHOT_MODE_AUTO && getShotMode() != SHOT_MODE_NIGHT)) {
        if (getRecordingHint() == true
            || (getShotMode() != SHOT_MODE_BEAUTY_FACE
                && getShotMode() != SHOT_MODE_OUTFOCUS
                && getShotMode() != SHOT_MODE_SELFIE_ALARM
                && getShotMode() != SHOT_MODE_PRO_MODE)) {
            CLOGD("zsl multi-frame mode off for special shot %d", getShotMode());
            return;
        }
    }

#ifdef SR_CAPTURE
    if (getSROn() == true) {
        CLOGD("zsl multi-frame mode off (SR mode)");
        return;
    }
#endif

    if (getSeriesShotMode() == SERIES_SHOT_MODE_NONE && m_flashMgr->getNeedCaptureFlash()
#ifdef FLASHED_LLS_CAPTURE
    && !getFlashedLLSCapture()
#endif
        ) {
        CLOGD("zsl multi-frame mode off for flash single capture");
        return;
    }

    setZslMultiframeModeOn(true);
}

void ExynosCamera1Parameters::setZslMultiframeModeOn(bool enable)
{
    m_flagZslMultiframeOn = enable;
}

bool ExynosCamera1Parameters::getZslMultiframeModeOn(void)
{
    return m_flagZslMultiframeOn;
}
#endif
#endif

#ifdef LLS_STUNR
void ExynosCamera1Parameters::checkLLSStunrMode(void)
{
    m_flagLLSStunrMode = false;

    setLLSStunrMode(true);
    CLOGD("STUNR : set STUNR mode");

#ifdef OIS_CAPTURE
    if (getOISCaptureModeOn() == true) {
        setLLSStunrMode(true);
        /* reset OISCapture mode */
        ExynosCameraActivitySpecialCapture *captureMgr = m_activityControl->getSpecialCaptureMgr();
        captureMgr->resetOISCaptureFcount();
        setOISCaptureModeOn(false);
        CLOGD("STUNR : reset OIS Capture mode");
    }
#ifdef SAMSUNG_LLS_DEBLUR
    /* LDCapture mode */
    if (getLDCaptureMode() > 0) {
        setLLSStunrMode(true);
        /* reset LDCapture mode */
        setLDCaptureMode(MULTI_SHOT_MODE_NONE);
        if (m_cameraId == CAMERA_ID_BACK || m_cameraId == CAMERA_ID_BACK_1)
            setOutPutFormatNV21Enable(false);
        CLOGD("STUNR : reset LD Capture mode");
    }
#endif
#else
    //TODO : Set LLSStunrMode using LLS value
#endif
}

void ExynosCamera1Parameters::setLLSStunrMode(bool enable)
{
    m_flagLLSStunrMode = enable;
    if (enable)
        m_LLSStunrCaptureCount = LLS_STUNR_CAPTURE_COUNT;
    else
        m_LLSStunrCaptureCount = 0;
}

bool ExynosCamera1Parameters::getLLSStunrMode(void)
{
    return m_flagLLSStunrMode;
}

int ExynosCamera1Parameters::getLLSStunrCount(void)
{
    return m_LLSStunrCaptureCount;
}
#endif

#ifdef USE_ODC_CAPTURE
status_t ExynosCamera1Parameters::checkODCCaptureMode()
{
    if(getDualMode() == true) {//PIP mode
        setODCCaptureMode(USE_ODC_CAPTURE_DUAL);
    }
    else {//Single camera mode
        setODCCaptureMode(USE_ODC_CAPTURE_SINGLE);
    }

#ifdef USE_DUAL_CAMERA
    if (getDualMode() == true && getDualCameraMode() == true) //Dual camera mode
        setODCCaptureMode(USE_ODC_CAPTURE_DUAL);
#endif

    return NO_ERROR;
}

void ExynosCamera1Parameters::setODCCaptureMode(bool enable)
{
    m_flagODCCaptureMode = enable;
}

bool ExynosCamera1Parameters::getODCCaptureMode(void)
{
    return m_flagODCCaptureMode;
}

void ExynosCamera1Parameters::setODCCaptureEnable(bool enable)
{
    m_flagODCCaptureEnable = enable;
}

bool ExynosCamera1Parameters::getODCCaptureEnable(void)
{
    return m_flagODCCaptureEnable;
}
#endif

#ifdef SAMSUNG_LENS_DC
status_t ExynosCamera1Parameters::checkLensDCMode(const CameraParameters& params)
{
    const char *newLensDCMode = params.get("rear-lens-distortion-correction");
    bool lensDCMode = false;

    if (newLensDCMode != NULL) {
        CLOGD("[setParameters]newLensDCMode %s", newLensDCMode);

        if (!strcmp(newLensDCMode, "on"))
            lensDCMode = true;

        setLensDCMode(lensDCMode);
        m_params.set("rear-lens-distortion-correction", newLensDCMode);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::setLensDCMode(bool mode)
{
    m_flagLensDCMode = mode;
}

bool ExynosCamera1Parameters::getLensDCMode(void)
{
    return m_flagLensDCMode;
}

void ExynosCamera1Parameters::setLensDCEnable(bool enable)
{
    m_flagLensDCEnable = enable;
}

bool ExynosCamera1Parameters::getLensDCEnable(void)
{
    return m_flagLensDCEnable;
}
#endif

#ifdef SAMSUNG_STR_CAPTURE
bool ExynosCamera1Parameters::checkSTRCaptureMode(void)
{
    if ((getCameraId() != CAMERA_ID_FRONT || getCameraId() != CAMERA_ID_FRONT_1)
        || getZoomLevel() > 0
        || getRecordingHint() == true) {
        CLOGD("[STR_CAPTURE] %s[%d]: false", __FUNCTION__, __LINE__);
        return false;
    }

    if (getSamsungCamera() == true) {
        if (getSeriesShotMode() != SERIES_SHOT_MODE_BURST
            && (getShotMode() == SHOT_MODE_AUTO
                || getShotMode() == SHOT_MODE_BEAUTY_FACE
                || getShotMode() == SHOT_MODE_NIGHT)) {
            CLOGD("[STR_CAPTURE] %s[%d]: true", __FUNCTION__, __LINE__);
            return true;
        } else {
            CLOGD("[STR_CAPTURE] %s[%d]: false", __FUNCTION__, __LINE__);
            return false;
        }
    } else { // default on for 3rd party app
        CLOGD("[STR_CAPTURE] %s[%d]: true", __FUNCTION__, __LINE__);
        return true;
    }

    return false;
}

void ExynosCamera1Parameters::setSTRCaptureEnable(bool enable)
{
    m_flagSTRCaptureEnable = enable;
}

bool ExynosCamera1Parameters::getSTRCaptureEnable(void)
{
    return m_flagSTRCaptureEnable;
}
#endif

#ifdef SAMSUNG_STR_PREVIEW
bool ExynosCamera1Parameters::checkSTRPreviewMode(void)
{
    if ((getCameraId() != CAMERA_ID_FRONT || getCameraId() != CAMERA_ID_FRONT_1)
        || getZoomLevel() > 0
        || getRecordingHint() == true) {
        return false;
    }

    if (getSamsungCamera() == true) {
        if (getSeriesShotMode() != SERIES_SHOT_MODE_BURST
            && (getShotMode() == SHOT_MODE_AUTO
                || getShotMode() == SHOT_MODE_BEAUTY_FACE
                || getShotMode() == SHOT_MODE_NIGHT)) {
            return true;
        } else {
            return false;
        }
    } else { // default on for 3rd party app
        return true;
    }

    return false;
}
#endif

#ifdef SAMSUNG_FOCUS_PEAKING
void ExynosCamera1Parameters::m_setFocusPeakingMode(bool enable)
{
    m_flagFocusPeakingMode = enable;
}

bool ExynosCamera1Parameters::m_getFocusPeakingMode(void)
{
    return m_flagFocusPeakingMode;
}
#endif

#ifdef SAMSUNG_IDDQD
bool ExynosCamera1Parameters::checkIDDQDMode(void)
{
    if (getRecordingHint() == true) {
        return false;
    }

    if (getSamsungCamera() == true) {
        if (getShotMode() == SHOT_MODE_AUTO
            || getShotMode() == SHOT_MODE_BEAUTY_FACE
            || getShotMode() == SHOT_MODE_NIGHT) {
            return true;
        }
    }
    return false;
}
#endif

void ExynosCamera1Parameters::setOutPutFormatNV21Enable(bool enable)
{
    m_flagOutPutFormatNV21Enable = enable;
}

bool ExynosCamera1Parameters::getOutPutFormatNV21Enable(void)
{
    return m_flagOutPutFormatNV21Enable;
}

void ExynosCamera1Parameters::m_getSetfileYuvRange(bool flagReprocessing, int *setfile, int *yuvRange)
{
    uint32_t currentSetfile = 0;
    uint32_t currentScenario = 0;
    uint32_t stateReg = 0;
    int flagYUVRange = YUV_FULL_RANGE;

    unsigned int minFps = 0;
    unsigned int maxFps = 0;
    getPreviewFpsRange(&minFps, &maxFps);

/* This code is for FHD 60fps performance issue in Grace */
#if 0 //def SUPPORT_SW_VDIS
    if (isSWVdisMode()) {
        currentScenario = FIMC_IS_SCENARIO_SWVDIS;
    }
#endif

#ifdef SAMSUNG_LIVE_OUTFOCUS
    if (getShotMode() == SHOT_MODE_LIVE_OUTFOCUS)
        stateReg |= STATE_REG_LIVE_OUTFOCUS;
#endif

#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true) {
        currentScenario = FIMC_IS_SCENARIO_AUTO_DUAL;
    }
#endif

#ifdef SAMSUNG_COLOR_IRIS
    if (getShotMode() == SHOT_MODE_COLOR_IRIS)
        currentScenario = FIMC_IS_SCENARIO_COLOR_IRIS;
#endif

#ifdef SAMSUNG_COMPANION
    if (getRTHdr() == COMPANION_WDR_ON)
        stateReg |= STATE_REG_RTHDR_ON;
    else if (getRTHdr() == COMPANION_WDR_AUTO
#ifdef SUPPORT_WDR_AUTO_LIKE
            || getRTHdr() == COMPANION_WDR_AUTO_LIKE
#endif
        )
        stateReg |= STATE_REG_RTHDR_AUTO;
#endif

    if (m_isUHDRecordingMode() == true)
        stateReg |= STATE_REG_UHD_RECORDING;

	if (getDualMode() == true
#ifdef USE_DUAL_CAMERA
            && getDualCameraMode() == false
#endif
        ) {
        stateReg |= STATE_REG_DUAL_MODE;
        if (getDualRecordingHint() == true)
            stateReg |= STATE_REG_DUAL_RECORDINGHINT;
    } else {
        if (getRecordingHint() == true)
            stateReg |= STATE_REG_RECORDINGHINT;
    }

    if (flagReprocessing == true) {
        stateReg |= STATE_REG_FLAG_REPROCESSING;

#ifdef SET_LLS_CAPTURE_SETFILE
        if (getLLSCaptureOn() == true) {
            stateReg |= STATE_REG_NEED_LLS;

            int zoomLevel = getZoomLevel();
            float zoomRatio = getZoomRatio(zoomLevel) / 1000;

            if (zoomRatio > 1.0f) {
                stateReg |= STATE_REG_ZOOM;
                CLOGV("currentSetfile zoom");
            }
        }
#endif

        if (getLongExposureShotCount() > 0)
            stateReg |= STATE_STILL_CAPTURE_LONG;
#ifdef LLS_CAPTURE
        else if (getLongExposureShotCount() == 0
                 && getLLSValue() == LLS_LEVEL_MANUAL_ISO)
            stateReg |= STATE_REG_MANUAL_ISO;
#ifdef SAMSUNG_COMPANION
        if (getRTHdr() == COMPANION_WDR_AUTO
            && getLLSValue() == LLS_LEVEL_SHARPEN_SINGLE)
            stateReg |= STATE_REG_SHARPEN_SINGLE;
#endif
#endif
#ifdef SR_CAPTURE
        if (getRecordingHint() == false
#ifdef SET_LLS_CAPTURE_SETFILE
            && getLLSCaptureOn() == false
#endif
            && getSROn() == true
            && getLLSValue() != LLS_LEVEL_SHARPEN_SINGLE
            && getLLSValue() != LLS_LEVEL_MANUAL_ISO
            && getLongExposureShotCount() == 0) {
            int zoomLevel = getZoomLevel();
            float zoomRatio = getZoomRatio(zoomLevel) / 1000;

            if (zoomRatio > 1.0f) {
                stateReg |= STATE_REG_ZOOM;
                CLOGV("currentSetfile zoom");
            }
        }
#endif
    } else if (flagReprocessing == false) {
        if (stateReg & STATE_REG_RECORDINGHINT
            || stateReg & STATE_REG_UHD_RECORDING
            || stateReg & STATE_REG_DUAL_RECORDINGHINT
#ifdef SAMSUNG_COLOR_IRIS
            || getShotMode() == SHOT_MODE_COLOR_IRIS
#endif
           ) {
            flagYUVRange = YUV_LIMITED_RANGE;
        }

#ifdef USE_BINNING_MODE
        if (getBinningMode() == true) {
            stateReg |= STATE_REG_BINNING_MODE;
        }
#endif
    }

    if (m_cameraId == CAMERA_ID_FRONT || m_cameraId == CAMERA_ID_FRONT_1) {
        int vtMode = getVtMode();

        if (vtMode > 0) {
            switch (vtMode) {
            case 1:
                currentSetfile = ISS_SUB_SCENARIO_FRONT_VT1;
                if (stateReg == STATE_STILL_CAPTURE)
                    currentSetfile = ISS_SUB_SCENARIO_FRONT_VT1_STILL_CAPTURE;
                break;
            case 2:
                currentSetfile = ISS_SUB_SCENARIO_FRONT_VT2;
                break;
            case 4:
                currentSetfile = ISS_SUB_SCENARIO_FRONT_VT4;
                break;
            default:
                currentSetfile = ISS_SUB_SCENARIO_FRONT_VT2;
                break;
            }
        } else if (getIntelligentMode() == 1) {
            currentSetfile = ISS_SUB_SCENARIO_FRONT_SMART_STAY;
        } else if (getShotMode() == SHOT_MODE_FRONT_PANORAMA || getShotMode() == SHOT_MODE_INTERACTIVE) {
            currentSetfile = ISS_SUB_SCENARIO_FRONT_PANORAMA;
#ifdef SAMSUNG_COLOR_IRIS
        } else if (getShotMode() == SHOT_MODE_COLOR_IRIS) {
            currentSetfile = ISS_SUB_SCENARIO_FRONT_COLOR_IRIS_PREVIEW;
#endif
        } else {
            switch(stateReg) {
                case STATE_STILL_PREVIEW:
#if FRONT_CAMERA_USE_SAMSUNG_COMPANION
                    if (!getUseCompanion())
                        currentSetfile = ISS_SUB_SCENARIO_FRONT_C2_OFF_STILL_PREVIEW;
                    else
#endif
                        currentSetfile = ISS_SUB_SCENARIO_STILL_PREVIEW;
                    break;

                case STATE_STILL_PREVIEW_WDR_ON:
                    currentSetfile = ISS_SUB_SCENARIO_STILL_PREVIEW_WDR_ON;
                    break;

                case STATE_STILL_PREVIEW_WDR_AUTO:
                    currentSetfile = ISS_SUB_SCENARIO_STILL_PREVIEW_WDR_AUTO;
                    break;

                case STATE_VIDEO:
#if FRONT_CAMERA_USE_SAMSUNG_COMPANION
                    if (!getUseCompanion())
                        currentSetfile = ISS_SUB_SCENARIO_FRONT_C2_OFF_VIDEO;
                    else
#endif
                        currentSetfile = ISS_SUB_SCENARIO_VIDEO;
                    break;

                case STATE_VIDEO_WDR_ON:
                case STATE_UHD_VIDEO_WDR_ON:
                    currentSetfile = ISS_SUB_SCENARIO_VIDEO_WDR_ON;
                    break;

                case STATE_VIDEO_WDR_AUTO:
                case STATE_UHD_VIDEO_WDR_AUTO:
                    currentSetfile = ISS_SUB_SCENARIO_VIDEO_WDR_AUTO;
                    break;

                case STATE_STILL_CAPTURE:
                case STATE_VIDEO_CAPTURE:
                case STATE_UHD_PREVIEW_CAPTURE:
                case STATE_UHD_VIDEO_CAPTURE:
#if FRONT_CAMERA_USE_SAMSUNG_COMPANION
                    if (!getUseCompanion())
                        currentSetfile = ISS_SUB_SCENARIO_FRONT_C2_OFF_STILL_CAPTURE;
                    else
#endif
                        currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE;
                    break;

                case STATE_STILL_CAPTURE_WDR_ON:
                case STATE_VIDEO_CAPTURE_WDR_ON:
                case STATE_UHD_PREVIEW_CAPTURE_WDR_ON:
                case STATE_UHD_VIDEO_CAPTURE_WDR_ON:
                    currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_ON;
                    break;

                case STATE_STILL_CAPTURE_WDR_AUTO:
                case STATE_VIDEO_CAPTURE_WDR_AUTO:
                case STATE_UHD_PREVIEW_CAPTURE_WDR_AUTO:
                case STATE_UHD_VIDEO_CAPTURE_WDR_AUTO:
                    currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_AUTO;
                    break;

                case STATE_DUAL_STILL_CAPTURE:
                case STATE_DUAL_VIDEO_CAPTURE:
                    currentSetfile = ISS_SUB_SCENARIO_DUAL_STILL;
                    break;

                case STATE_DUAL_STILL_PREVIEW:
                    currentSetfile = ISS_SUB_SCENARIO_DUAL_PREVIEW;
                    break;

                case STATE_DUAL_VIDEO:
                    currentSetfile = ISS_SUB_SCENARIO_DUAL_VIDEO;
                    break;

                case STATE_UHD_PREVIEW:
                case STATE_UHD_VIDEO:
                    currentSetfile = ISS_SUB_SCENARIO_UHD_30FPS;
                    break;

                case STATE_STILL_BINNING_PREVIEW:
                case STATE_VIDEO_BINNING:
                case STATE_DUAL_VIDEO_BINNING:
                case STATE_DUAL_STILL_BINING_PREVIEW:
#if defined(USE_BINNING_MODE) && defined(SET_PREVIEW_BINNING_SETFILE)
                    currentSetfile = ISS_SUB_SCENARIO_FRONT_STILL_PREVIEW_BINNING;
#else
                    currentSetfile = ISS_SUB_SCENARIO_FRONT_STILL_PREVIEW_BINNING;
#endif
                    break;

#ifdef SET_LLS_CAPTURE_SETFILE
#if (LLS_SETFILE_VERSION == 2)
                case STATE_STILL_CAPTURE_LLS:
                    currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE;
                    break;
                case STATE_VIDEO_CAPTURE_WDR_ON_LLS:
                    currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE_WDR_ON;
                    break;
                case STATE_STILL_CAPTURE_WDR_AUTO_LLS:
                    currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE_WDR_AUTO;
                    break;
#else
                case STATE_STILL_CAPTURE_LLS:
                    currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_LLS;
#ifdef LLS_REPROCESSING
                    switch(getLLSCaptureCount()) {
                        case 1:
                            currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE;
                            break;
                        default:
                            break;
                    }
#endif
                    break;
                case STATE_VIDEO_CAPTURE_WDR_ON_LLS:
                    currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_ON_LLS;
#ifdef LLS_REPROCESSING
                    switch(getLLSCaptureCount()) {
                        case 1:
                            currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_ON;
                            break;
                        default:
                            break;
                    }
#endif
                    break;
                case STATE_STILL_CAPTURE_WDR_AUTO_LLS:
                    currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_LLS;
#ifdef LLS_REPROCESSING
                    switch(getLLSCaptureCount()) {
                        case 1:
                            currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_AUTO;
                            break;
                        default:
                            break;
                    }
#endif
                    break;
#endif /* LLS_SETFILE_VERSION == 2 */
#endif /* SET_LLS_CAPTURE_SETFILE */

                default:
                    CLOGD("can't define senario of setfile.(0x%4x)", stateReg);
                    break;
            }
        }
    } else {
        switch(stateReg) {
            case STATE_STILL_PREVIEW:
                currentSetfile = ISS_SUB_SCENARIO_STILL_PREVIEW;
                break;

            case STATE_STILL_PREVIEW_WDR_ON:
                currentSetfile = ISS_SUB_SCENARIO_STILL_PREVIEW_WDR_ON;
                break;

            case STATE_STILL_PREVIEW_WDR_AUTO:
                currentSetfile = ISS_SUB_SCENARIO_STILL_PREVIEW_WDR_AUTO;
                break;

            case STATE_STILL_CAPTURE:
            case STATE_VIDEO_CAPTURE:
            case STATE_DUAL_STILL_CAPTURE:
            case STATE_DUAL_VIDEO_CAPTURE:
            case STATE_UHD_PREVIEW_CAPTURE:
            case STATE_UHD_VIDEO_CAPTURE:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE;
                break;

            case STATE_STILL_CAPTURE_WDR_ON:
            case STATE_VIDEO_CAPTURE_WDR_ON:
            case STATE_UHD_PREVIEW_CAPTURE_WDR_ON:
            case STATE_UHD_VIDEO_CAPTURE_WDR_ON:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_ON;
                break;

            case STATE_STILL_CAPTURE_WDR_AUTO:
            case STATE_VIDEO_CAPTURE_WDR_AUTO:
            case STATE_UHD_PREVIEW_CAPTURE_WDR_AUTO:
            case STATE_UHD_VIDEO_CAPTURE_WDR_AUTO:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_AUTO;
                break;

            case STATE_STILL_BINNING_PREVIEW:
            case STATE_VIDEO_BINNING:
            case STATE_DUAL_VIDEO_BINNING:
            case STATE_DUAL_STILL_BINING_PREVIEW:
#if defined(USE_BINNING_MODE) && defined(SET_PREVIEW_BINNING_SETFILE)
                currentSetfile = ISS_SUB_SCENARIO_STILL_PREVIEW_BINNING;
#else
                currentSetfile = BINNING_SETFILE_INDEX;
#endif
                break;

            case STATE_VIDEO:
                if (30 < minFps && 30 < maxFps) {
                    if (300 == minFps && 300 == maxFps) {
                        currentSetfile = ISS_SUB_SCENARIO_WVGA_300FPS;
                    } else if (60 == minFps && 60 == maxFps) {
                        currentSetfile = ISS_SUB_SCENARIO_FHD_60FPS;
                    } else if (240 == minFps && 240 == maxFps) {
                        currentSetfile = ISS_SUB_SCENARIO_FHD_240FPS;
                    } else {
                        currentSetfile = ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED;
                    }
                } else {
                    currentSetfile = ISS_SUB_SCENARIO_VIDEO;
                }
                break;

            case STATE_VIDEO_WDR_ON:
                currentSetfile = ISS_SUB_SCENARIO_VIDEO_WDR_ON;
                break;

            case STATE_VIDEO_WDR_AUTO:
                if (30 < minFps && 30 < maxFps) {
                    if (300 == minFps && 300 == maxFps) {
                        currentSetfile = ISS_SUB_SCENARIO_WVGA_300FPS;
                    } else if (60 == minFps && 60 == maxFps) {
                        currentSetfile = ISS_SUB_SCENARIO_FHD_60FPS;
                    } else if (240 == minFps && 240 == maxFps) {
                        currentSetfile = ISS_SUB_SCENARIO_FHD_240FPS;
                    } else {
                        currentSetfile = ISS_SUB_SCENARIO_VIDEO_HIGH_SPEED;
                    }
                } else {
                    currentSetfile = ISS_SUB_SCENARIO_VIDEO_WDR_AUTO;
                }
                break;

            case STATE_DUAL_VIDEO:
                currentSetfile = ISS_SUB_SCENARIO_DUAL_VIDEO;
                break;

            case STATE_DUAL_STILL_PREVIEW:
                currentSetfile = ISS_SUB_SCENARIO_DUAL_STILL;
                break;

            case STATE_UHD_PREVIEW:
            case STATE_UHD_VIDEO:
            case STATE_UHD_VIDEO_WDR_AUTO:
                currentSetfile = ISS_SUB_SCENARIO_UHD_30FPS;
                break;

            case STATE_UHD_PREVIEW_WDR_ON:
            case STATE_UHD_VIDEO_WDR_ON:
                currentSetfile = ISS_SUB_SCENARIO_UHD_30FPS_WDR_ON;
                break;

            case STATE_STILL_CAPTURE_WDR_ON_ZOOM:
            case STATE_STILL_CAPTURE_WDR_ON_LLS_ZOOM:
                currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE_WDR_ON;
                break;
            case STATE_STILL_CAPTURE_ZOOM:
            case STATE_STILL_CAPTURE_LLS_ZOOM:
            case STATE_STILL_CAPTURE_WDR_AUTO_ZOOM:
            case STATE_STILL_CAPTURE_WDR_AUTO_LLS_ZOOM:
                currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE_WDR_AUTO;
                break;

#ifdef SET_LLS_CAPTURE_SETFILE
#if (LLS_SETFILE_VERSION == 2)
            case STATE_STILL_CAPTURE_LLS:
                currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE;
                break;
            case STATE_VIDEO_CAPTURE_WDR_ON_LLS:
                currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE_WDR_ON;
                break;
            case STATE_STILL_CAPTURE_WDR_AUTO_LLS:
                currentSetfile = ISS_SUB_SCENARIO_MERGED_STILL_CAPTURE_WDR_AUTO;
                break;
#else
            case STATE_STILL_CAPTURE_LLS:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_LLS;
#ifdef LLS_REPROCESSING
                switch(getLLSCaptureCount()) {
                    case 1:
                        currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE;
                        break;
                    default:
                        break;
                }
#endif
                break;
            case STATE_VIDEO_CAPTURE_WDR_ON_LLS:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_ON_LLS;
#ifdef LLS_REPROCESSING
                switch(getLLSCaptureCount()) {
                    case 1:
                        currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_ON;
                        break;
                    default:
                        break;
                }
#endif
                break;
            case STATE_STILL_CAPTURE_WDR_AUTO_LLS:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_LLS;
#ifdef LLS_REPROCESSING
                switch(getLLSCaptureCount()) {
                    case 1:
                        currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_WDR_AUTO;
                        break;
                    default:
                        break;
                }
#endif
                break;
#endif /* LLS_SETFILE_VERSION == 2 */
#endif /* SET_LLS_CAPTURE_SETFILE */
            case STATE_STILL_CAPTURE_WDR_AUTO_SHARPEN:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_SHARPEN;
                break;
            case STATE_STILL_CAPTURE_LONG:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_LONG;
                break;
            case STATE_STILL_CAPTURE_MANUAL_ISO:
                currentSetfile = ISS_SUB_SCENARIO_STILL_CAPTURE_MANUAL_ISO;
                break;
#ifdef SAMSUNG_LIVE_OUTFOCUS
            case STATE_LIVE_OUTFOCUS_PREVIEW:
                currentSetfile = ISS_SUB_SCENARIO_LIVE_OUTFOCUS_PREVIEW;
                break;
            case STATE_LIVE_OUTFOCUS_CAPTURE:
                currentSetfile = ISS_SUB_SCENARIO_LIVE_OUTFOCUS_CAPTURE;
                break;
            case STATE_LIVE_OUTFOCUS_VIDEO:
                currentSetfile = ISS_SUB_SCENARIO_LIVE_OUTFOCUS_VIDEO;
                break;
#endif
            default:
                CLOGD("can't define senario of setfile.(0x%4x)", stateReg);
                break;
        }
    }
#if 0
    CLOGD("===============================================================================");
    CLOGD("CurrentState(0x%4x)", stateReg);
    CLOGD("getRTHdr()(%d)", getRTHdr());
    CLOGD("getRecordingHint()(%d)", getRecordingHint());
    CLOGD("m_isUHDRecordingMode()(%d)", m_isUHDRecordingMode());
    CLOGD("getDualMode()(%d)", getDualMode());
    CLOGD("getDualRecordingHint()(%d)", getDualRecordingHint());
#ifdef USE_BINNING_MODE
    CLOGD("getBinningMode(%d)", getBinningMode());
#endif
    CLOGD("flagReprocessing(%d)", flagReprocessing);
    CLOGD("===============================================================================");
    CLOGD("currentSetfile(%d)", currentSetfile);
    CLOGD("flagYUVRange(%d)", flagYUVRange);
    CLOGD("===============================================================================");
#else
    CLOGD("CurrentState (0x%4x), currentSetfile(%d), currentScenario(%d",
            stateReg, currentSetfile, currentScenario);
#endif

    *setfile = currentSetfile | (currentScenario << 16);
    *yuvRange = flagYUVRange;
}

#ifdef SAMSUNG_LLV
void ExynosCamera1Parameters::setLLV(bool enable)
{
    m_isLLVOn = enable;
}

bool ExynosCamera1Parameters::getLLV(void)
{
    bool isSizeSupported = true;
    int videoW, videoH;

#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true) {
        ExynosRect fusionSrcRect;
        ExynosRect fusionDstRect;

        if (getFusionSize(hwPreviewW, hwPreviewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
            CLOGE("getFusionSize(%d, %d) fail", hwPreviewW, hwPreviewH);
        } else {
            hwPreviewW = fusionDstRect.w;
            hwPreviewH = fusionDstRect.h;
        }
    }
#endif

    getVideoSize(&videoW, &videoH);
    if(videoW > 1920 || videoH > 1080)
        isSizeSupported = false;
    else {
        uint32_t minFPS = 0;
        uint32_t maxFPS = 0;
        getPreviewFpsRange(&minFPS, &maxFPS);
        if(minFPS > 30 || maxFPS > 30)
            isSizeSupported = false;
    }

    return (m_isLLVOn && isSizeSupported);
}
#endif

status_t ExynosCamera1Parameters::calcPreviewGSCRect(ExynosRect *srcRect, ExynosRect *dstRect)
{
    int ret = 0;

    int cropX = 0, cropY = 0;
    int cropW = 0, cropH = 0;

    int previewW = 0, previewH = 0, previewFormat = 0;
    int hwPreviewW = 0, hwPreviewH = 0, hwPreviewFormat = 0;
    previewFormat = getPreviewFormat();
    hwPreviewFormat = getHwPreviewFormat();

    getHwPreviewSize(&hwPreviewW, &hwPreviewH);
    getPreviewSize(&previewW, &previewH);

#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true) {
        ExynosRect fusionSrcRect;
        ExynosRect fusionDstRect;

        if (getFusionSize(hwPreviewW, hwPreviewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
            CLOGE("getFusionSize(%d, %d) fail", hwPreviewW, hwPreviewH);
        } else {
            hwPreviewW = fusionDstRect.w;
            hwPreviewH = fusionDstRect.h;
        }
    }
#endif

    ret = getCropRectAlign(hwPreviewW, hwPreviewH,
                           previewW, previewH,
                           &cropX, &cropY,
                           &cropW, &cropH,
                           GSCALER_IMG_ALIGN, 2,
                           0, 1);

    srcRect->x = cropX;
    srcRect->y = cropY;
    srcRect->w = cropW;
    srcRect->h = cropH;
    srcRect->fullW = cropW;
    srcRect->fullH = cropH;
    srcRect->colorFormat = hwPreviewFormat;

#if defined(USE_GSC_FOR_PREVIEW)
#if defined(SAMSUNG_HYPER_MOTION)
    if (getHyperMotionMode() == true) {
        int hyperMotion_OutW, hyperMotion_OutH;

        hyperMotion_OutW = hwPreviewW;
        hyperMotion_OutH = hwPreviewH;
        m_hyperMotion_AdjustPreviewSize(&hyperMotion_OutW, &hyperMotion_OutH);

        srcRect->x = (hwPreviewW - hyperMotion_OutW) / 2;
        srcRect->y = (hwPreviewH - hyperMotion_OutH) / 2;
        srcRect->w = hyperMotion_OutW;
        srcRect->h = hyperMotion_OutH;
        srcRect->fullW = hwPreviewW;
        srcRect->fullH = hwPreviewH;
        srcRect->colorFormat = hwPreviewFormat;
    }
#endif
#if defined(SUPPORT_SW_VDIS)
    if (isSWVdisMode() == true) {
        int swVDIS_OutW, swVDIS_OutH;

        swVDIS_OutW = hwPreviewW;
        swVDIS_OutH = hwPreviewH;
        m_swVDIS_AdjustPreviewSize(&swVDIS_OutW, &swVDIS_OutH);

        srcRect->x = (hwPreviewW - swVDIS_OutW) / 2;
        srcRect->y = (hwPreviewH - swVDIS_OutH) / 2;
        srcRect->w = swVDIS_OutW;
        srcRect->h = swVDIS_OutH;
        srcRect->fullW = hwPreviewW;
        srcRect->fullH = hwPreviewH;
        srcRect->colorFormat = hwPreviewFormat;
    }
#endif
#endif

    dstRect->x = 0;
    dstRect->y = 0;
    dstRect->w = previewW;
    dstRect->h = previewH;
    dstRect->fullW = previewW;
    dstRect->fullH = previewH;
    dstRect->colorFormat = previewFormat;

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::calcRecordingGSCRect(ExynosRect *srcRect, ExynosRect *dstRect)
{
    int ret = 0;

    int cropX = 0, cropY = 0;
    int cropW = 0, cropH = 0;

    int hwPreviewW = 0, hwPreviewH = 0, hwPreviewFormat = 0;
    int videoW = 0, videoH = 0, videoFormat = 0;
    float zoomRatio = getZoomRatio(0) / 1000;

    hwPreviewFormat = getHwPreviewFormat();
    videoFormat     = getVideoFormat();

    getHwPreviewSize(&hwPreviewW, &hwPreviewH);

#ifdef USE_DUAL_CAMERA
    if (getDualCameraMode() == true) {
        ExynosRect fusionSrcRect;
        ExynosRect fusionDstRect;

        if (getFusionSize(hwPreviewW, hwPreviewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
            CLOGE("getFusionSize(%d, %d) fail", hwPreviewW, hwPreviewH);
        } else {
            hwPreviewW = fusionDstRect.w;
            hwPreviewH = fusionDstRect.h;
        }
    }
#endif

#ifdef SUPPORT_SW_VDIS
    if (isSWVdisMode() == true) {
        m_swVDIS_AdjustPreviewSize(&hwPreviewW, &hwPreviewH);
    }
#endif

    getVideoSize(&videoW, &videoH);

    if (hwPreviewW < videoW || hwPreviewH < videoH) {
        cropW = hwPreviewW;
        cropH = hwPreviewH;
    } else  {
        ret = getCropRectAlign(hwPreviewW, hwPreviewH,
                         videoW, videoH,
                         &cropX, &cropY,
                         &cropW, &cropH,
                         2, 2,
                         0, zoomRatio);
    }

    srcRect->x = cropX;
    srcRect->y = cropY;
    srcRect->w = cropW;
    srcRect->h = cropH;
    srcRect->fullW = hwPreviewW;
    srcRect->fullH = hwPreviewH;
    srcRect->colorFormat = hwPreviewFormat;

    dstRect->x = 0;
    dstRect->y = 0;
    dstRect->w = videoW;
    dstRect->h = videoH;
    dstRect->fullW = videoW;
    dstRect->fullH = videoH;
    dstRect->colorFormat = videoFormat;

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::calcPreviewBayerCropSize(ExynosRect *srcRect, ExynosRect *dstRect)
{
    int ret = 0;

    int hwSensorW = 0, hwSensorH = 0;
    int hwPictureW = 0, hwPictureH = 0;
    int pictureW = 0, pictureH = 0;
    int previewW = 0, previewH = 0;
    int hwSensorMarginW = 0, hwSensorMarginH = 0;

    int cropX = 0, cropY = 0;
    int cropW = 0, cropH = 0;
#if 0
    int crop_crop_x = 0, crop_crop_y = 0;
    int crop_crop_w = 0, crop_crop_h = 0;
    int pictureFormat = 0, hwPictureFormat = 0;
#endif
    int zoomLevel = 0;
    int maxZoomRatio = 0;
    int bayerFormat = getBayerFormat(PIPE_3AA);
    float zoomRatio = getZoomRatio(0) / 1000;

#ifdef DEBUG_RAWDUMP
    if (checkBayerDumpEnable()) {
        bayerFormat = CAMERA_DUMP_BAYER_FORMAT;
    }
#endif

    /* TODO: check state ready for start */
#if 0
    pictureFormat = getHwPictureFormat();
#endif

    if (isUse3aaInputCrop() == true) {
        zoomLevel = getZoomLevel();
        zoomRatio = getZoomRatio(zoomLevel) / 1000;
    }

    maxZoomRatio = getMaxZoomRatio() / 1000;
    getHwPictureSize(&hwPictureW, &hwPictureH);
    getPictureSize(&pictureW, &pictureH);

    getHwSensorSize(&hwSensorW, &hwSensorH);
    getPreviewSize(&previewW, &previewH);
    getSensorMargin(&hwSensorMarginW, &hwSensorMarginH);

    hwSensorW -= hwSensorMarginW;
    hwSensorH -= hwSensorMarginH;

    ret = getCropRectAlign(hwSensorW, hwSensorH,
                     previewW, previewH,
                     &cropX, &cropY,
                     &cropW, &cropH,
                     CAMERA_BCROP_ALIGN, 2,
                     zoomLevel, zoomRatio);

    cropX = ALIGN_DOWN(cropX, 2);
    cropY = ALIGN_DOWN(cropY, 2);
    cropW = hwSensorW - (cropX * 2);
    cropH = hwSensorH - (cropY * 2);

    if (getUsePureBayerReprocessing() == false) {
        int pictureCropX = 0, pictureCropY = 0;
        int pictureCropW = 0, pictureCropH = 0;

        zoomLevel = 0;
        zoomRatio = getZoomRatio(zoomLevel) / 1000;

        ret = getCropRectAlign(cropW, cropH,
                pictureW, pictureH,
                &pictureCropX, &pictureCropY,
                &pictureCropW, &pictureCropH,
                CAMERA_BCROP_ALIGN, 2,
                zoomLevel, zoomRatio);

        pictureCropX = ALIGN_DOWN(pictureCropX, 2);
        pictureCropY = ALIGN_DOWN(pictureCropY, 2);
        pictureCropW = cropW - (pictureCropX * 2);
        pictureCropH = cropH - (pictureCropY * 2);

        if (pictureCropW < pictureW / maxZoomRatio || pictureCropH < pictureH / maxZoomRatio) {
            CLOGW(" zoom ratio is upto x%d, crop(%dx%d), picture(%dx%d)", maxZoomRatio, cropW, cropH, pictureW, pictureH);
            float src_ratio = 1.0f;
            float dst_ratio = 1.0f;
            /* ex : 1024 / 768 */
            src_ratio = ROUND_OFF_HALF(((float)cropW / (float)cropH), 2);
            /* ex : 352  / 288 */
            dst_ratio = ROUND_OFF_HALF(((float)pictureW / (float)pictureH), 2);

            if (dst_ratio <= src_ratio) {
                /* shrink w */
                cropX = ALIGN_DOWN(((int)(hwSensorW - ((pictureH / maxZoomRatio) * src_ratio)) >> 1), 2);
                cropY = ALIGN_DOWN(((hwSensorH - (pictureH / maxZoomRatio)) >> 1), 2);
            } else {
                /* shrink h */
                cropX = ALIGN_DOWN(((hwSensorW - (pictureW / maxZoomRatio)) >> 1), 2);
                cropY = ALIGN_DOWN(((int)(hwSensorH - ((pictureW / maxZoomRatio) / src_ratio)) >> 1), 2);
            }
            cropW = hwSensorW - (cropX * 2);
            cropH = hwSensorH - (cropY * 2);
        }
    } else {
#ifdef USE_CP_FUSION_LIB
#ifdef USE_CP_FUSION_LIB_CROP_IN_LIBRARY
#else
#ifdef USE_CP_FUSION_LIB_LIMIT_CROP_ON_ISP
        if (this->isFusionEnabled() == true) {
            /*
             * when preview fusion,
             * bayer crop size need to be bigger than fusion input size.
             * for warping between wide/tele
             */
            int previewW = 0;
            int previewH = 0;

            this->getPreviewSize(&previewW, &previewH);

            ExynosRect fusionSrcRect;
            ExynosRect fusionDstRect;

            if (this->getFusionSize(previewW, previewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
                CLOGE("getFusionSize(%d, %d) fail", previewW, previewH);
                return INVALID_OPERATION;
            }

            if (cropW < fusionSrcRect.w) {
                CLOGV("calibrate bcrop width size (%d -> %d)",  cropW, fusionSrcRect.w);
                cropW = fusionSrcRect.w;
                cropX = ALIGN_DOWN(((hwSensorW - cropW) >> 1), 2);
            }

            if (cropH < fusionSrcRect.h) {
                CLOGV("calibrate bcrop height size (%d -> %d)", cropH, fusionSrcRect.h);
                cropH = fusionSrcRect.h;
                cropY = ALIGN_DOWN(((hwSensorH - cropH) >> 1), 2);
            }
        }
#endif // USE_CP_FUSION_LIB_LIMIT_CROP_ON_ISP
#endif // USE_CP_FUSION_LIB_CROP_IN_LIBRARY
#endif // USE_CP_FUSION_LIB
    }

#if 0
    CLOGD("hwSensorSize (%dx%d), previewSize (%dx%d)",
            hwSensorW, hwSensorH, previewW, previewH);
    CLOGD("hwPictureSize (%dx%d), pictureSize (%dx%d)",
            hwPictureW, hwPictureH, pictureW, pictureH);
    CLOGD("size cropX = %d, cropY = %d, cropW = %d, cropH = %d, zoom = %d",
            cropX, cropY, cropW, cropH, zoomLevel);
    CLOGD("size2 cropX = %d, cropY = %d, cropW = %d, cropH = %d, zoom = %d",
            crop_crop_x, crop_crop_y, crop_crop_w, crop_crop_h, zoomLevel);
    CLOGD("size pictureFormat = 0x%x, JPEG_INPUT_COLOR_FMT = 0x%x",
            pictureFormat, JPEG_INPUT_COLOR_FMT);
#endif

    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = hwSensorW;
    srcRect->h = hwSensorH;
    srcRect->fullW = hwSensorW;
    srcRect->fullH = hwSensorH;
    srcRect->colorFormat = bayerFormat;

    dstRect->x = cropX;
    dstRect->y = cropY;
    dstRect->w = cropW;
    dstRect->h = cropH;
    dstRect->fullW = cropW;
    dstRect->fullH = cropH;
    dstRect->colorFormat = bayerFormat;

    m_setHwBayerCropRegion(dstRect->w, dstRect->h, dstRect->x, dstRect->y);
    return NO_ERROR;
}

status_t ExynosCamera1Parameters::getPictureBdsSize(ExynosRect *dstRect)
{
    status_t ret = NO_ERROR;
    ExynosRect bnsSize;
    ExynosRect bayerCropSize;
    int hwBdsW = 0;
    int hwBdsH = 0;
    int acutalPictureRatioIdx = 0;

    acutalPictureRatioIdx = getActualPictureRatioIdx(m_cameraInfo.pictureSizeRatioId);

    /* matched ratio LUT is not existed, use equation */
    if (m_useSizeTable == false
        || m_staticInfo->pictureSizeLut == NULL
        || m_staticInfo->pictureSizeLutMax <= acutalPictureRatioIdx) {
        ExynosRect rect;
        return calcPictureBDSSize(&rect, dstRect);
    }

#ifdef USE_BINNING_MODE
    if (getBinningMode() == true) {
        /* use LUT */
        hwBdsW = m_staticInfo->vtcallSizeLut[acutalPictureRatioIdx][BDS_W];
        hwBdsH = m_staticInfo->vtcallSizeLut[acutalPictureRatioIdx][BDS_H];
    } else
#endif
#ifdef USE_REMOSAIC_CAPTURE
    if (getRemosaicCaptureMode() == REMOSAIC_CAPTURE_MODE_NONE) {
        /* Big Pixel sensor mode */
        hwBdsW = m_staticInfo->bpPictureSizeLut[acutalPictureRatioIdx][BDS_W];
        hwBdsH = m_staticInfo->bpPictureSizeLut[acutalPictureRatioIdx][BDS_H];
    } else
#endif
    {
        /* use LUT */
        hwBdsW = m_staticInfo->pictureSizeLut[acutalPictureRatioIdx][BDS_W];
        hwBdsH = m_staticInfo->pictureSizeLut[acutalPictureRatioIdx][BDS_H];
    }
    /* Check the invalid BDS size compared to Bcrop size */
    ret = getPictureBayerCropSize(&bnsSize, &bayerCropSize);
    if (ret != NO_ERROR)
        CLOGE("getPictureBayerCropSize() failed");

    if (bayerCropSize.w < hwBdsW || bayerCropSize.h < hwBdsH) {
        CLOGD("bayerCropSize %dx%d is smaller than BDSSize %dx%d. Force bayerCropSize",
                bayerCropSize.w, bayerCropSize.h, hwBdsW, hwBdsH);

        hwBdsW = bayerCropSize.w;
        hwBdsH = bayerCropSize.h;
    }

    dstRect->x = 0;
    dstRect->y = 0;
    dstRect->w = hwBdsW;
    dstRect->h = hwBdsH;

#ifdef DEBUG_PERFRAME
    CLOGD("hwBdsSize %dx%d", dstRect->w, dstRect->h);
#endif

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::getPictureBayerCropSize(ExynosRect *srcRect, ExynosRect *dstRect)
{
    int hwBnsW   = 0;
    int hwBnsH   = 0;
    int hwBcropW = 0;
    int hwBcropH = 0;
    int zoomLevel = 0;
    float zoomRatio = 1.00f;
    int hwSensorMarginW = 0;
    int hwSensorMarginH = 0;
    int acutalPictureRatioIdx = 0;

    acutalPictureRatioIdx = getActualPictureRatioIdx(m_cameraInfo.pictureSizeRatioId);

    /* matched ratio LUT is not existed, use equation */
    if (m_useSizeTable == false
        || m_staticInfo->pictureSizeLut == NULL
        || m_staticInfo->pictureSizeLutMax <= acutalPictureRatioIdx
        || m_cameraInfo.pictureSizeRatioId != m_cameraInfo.previewSizeRatioId)
        return calcPictureBayerCropSize(srcRect, dstRect);

#ifdef USE_BINNING_MODE
    if(getBinningMode() == true) {
        /* use LUT */
        hwBnsW   = m_staticInfo->vtcallSizeLut[acutalPictureRatioIdx][BNS_W];
        hwBnsH   = m_staticInfo->vtcallSizeLut[acutalPictureRatioIdx][BNS_H];
        hwBcropW = m_staticInfo->vtcallSizeLut[acutalPictureRatioIdx][BCROP_W];
        hwBcropH = m_staticInfo->vtcallSizeLut[acutalPictureRatioIdx][BCROP_H];
    } else
#endif
#ifdef USE_REMOSAIC_CAPTURE
    if (getRemosaicCaptureMode() == REMOSAIC_CAPTURE_MODE_NONE) {
        /* Big Pixel sensor mode */
        hwBnsW   = m_staticInfo->bpPictureSizeLut[acutalPictureRatioIdx][BNS_W];
        hwBnsH   = m_staticInfo->bpPictureSizeLut[acutalPictureRatioIdx][BNS_H];
        hwBcropW = m_staticInfo->bpPictureSizeLut[acutalPictureRatioIdx][BCROP_W];
        hwBcropH = m_staticInfo->bpPictureSizeLut[acutalPictureRatioIdx][BCROP_H];
    } else
#endif
    {
        /* use LUT */
        hwBnsW   = m_staticInfo->pictureSizeLut[acutalPictureRatioIdx][BNS_W];
        hwBnsH   = m_staticInfo->pictureSizeLut[acutalPictureRatioIdx][BNS_H];
        hwBcropW = m_staticInfo->pictureSizeLut[acutalPictureRatioIdx][BCROP_W];
        hwBcropH = m_staticInfo->pictureSizeLut[acutalPictureRatioIdx][BCROP_H];
    }
    if (isUseReprocessing3aaInputCrop() == true) {
#if defined(SR_CAPTURE) || defined(SAMSUNG_HIFI_LLS)
        if (getNoBayerCrop()) {
            zoomLevel = ZOOM_LEVEL_0;
        } else
#endif
        {
            zoomLevel = getZoomLevel();
        }
        zoomRatio = getZoomRatio(zoomLevel) / 1000;
    }

#if defined(SCALER_MAX_SCALE_UP_RATIO)
    /*
     * After dividing float & casting int,
     * zoomed size can be smaller too much.
     * so, when zoom until max, ceil up about floating point.
     */
    if (ALIGN_UP((int)((float)hwBcropW / zoomRatio), CAMERA_BCROP_ALIGN) * SCALER_MAX_SCALE_UP_RATIO < hwBcropW ||
        ALIGN_UP((int)((float)hwBcropH / zoomRatio), 2) * SCALER_MAX_SCALE_UP_RATIO < hwBcropH) {
        hwBcropW = ALIGN_UP((int)ceil((float)hwBcropW / zoomRatio), CAMERA_BCROP_ALIGN);
        hwBcropH = ALIGN_UP((int)ceil((float)hwBcropH / zoomRatio), 2);
    } else
#endif
    {
        hwBcropW = ALIGN_UP((int)((float)hwBcropW / zoomRatio), CAMERA_BCROP_ALIGN);
        hwBcropH = ALIGN_UP((int)((float)hwBcropH / zoomRatio), 2);
    }

    /* Re-calculate the BNS size for removing Sensor Margin.
       On Capture Stream(3AA_M2M_Input), the BNS is not used.
       So, the BNS ratio is not needed to be considered for sensor margin */
    getSensorMargin(&hwSensorMarginW, &hwSensorMarginH);
    hwBnsW = hwBnsW - hwSensorMarginW;
    hwBnsH = hwBnsH - hwSensorMarginH;

    /* src */
    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = hwBnsW;
    srcRect->h = hwBnsH;

    /* dst */
    if (hwBnsW > hwBcropW) {
        dstRect->x = ALIGN_UP(((hwBnsW - hwBcropW) >> 1), 2);
        dstRect->w = hwBcropW;
    } else {
        dstRect->x = 0;
        dstRect->w = hwBnsW;
    }

    if (hwBnsH > hwBcropH) {
        dstRect->y = ALIGN_UP(((hwBnsH - hwBcropH) >> 1), 2);
        dstRect->h = hwBcropH;
    } else {
        dstRect->y = 0;
        dstRect->h = hwBnsH;
    }


#if DEBUG
    CLOGE("hwBnsSize %dx%d, hwBcropSize %d,%d %dx%d zoomLevel %d zoomRatio %d",
            srcRect->w, srcRect->h,
            dstRect->x, dstRect->y, dstRect->w, dstRect->h,
            zoomLevel, zoomRatio);
#endif

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::calcPictureBayerCropSize(ExynosRect *srcRect, ExynosRect *dstRect)
{
    int ret = 0;

    int maxSensorW = 0, maxSensorH = 0;
    int hwSensorW = 0, hwSensorH = 0;
    int hwPictureW = 0, hwPictureH = 0;
    int hwSensorCropX = 0, hwSensorCropY = 0;
    int hwSensorCropW = 0, hwSensorCropH = 0;
    int pictureW = 0, pictureH = 0, pictureFormat = 0;
    int previewW = 0, previewH = 0;
    int hwSensorMarginW = 0, hwSensorMarginH = 0;

    int cropX = 0, cropY = 0;
    int cropW = 0, cropH = 0;
    int crop_crop_x = 0, crop_crop_y = 0;
    int crop_crop_w = 0, crop_crop_h = 0;

    int zoomLevel = 0;
    float zoomRatio = 1.0f;
    int maxZoomRatio = 0;
    int bayerFormat = getBayerFormat(PIPE_3AA_REPROCESSING);

#ifdef DEBUG_RAWDUMP
    if (checkBayerDumpEnable()) {
        bayerFormat = CAMERA_DUMP_BAYER_FORMAT;
    }
#endif

    /* TODO: check state ready for start */
    if (isUseReprocessing3aaInputCrop() == true) {
        zoomLevel = getZoomLevel();
        zoomRatio = getZoomRatio(zoomLevel) / 1000;
    }

    pictureFormat = getHwPictureFormat();
    maxZoomRatio = getMaxZoomRatio() / 1000;
    getHwPictureSize(&hwPictureW, &hwPictureH);
    getPictureSize(&pictureW, &pictureH);

    getMaxSensorSize(&maxSensorW, &maxSensorH);
    getHwSensorSize(&hwSensorW, &hwSensorH);
    getPreviewSize(&previewW, &previewH);
    getSensorMargin(&hwSensorMarginW, &hwSensorMarginH);


    hwSensorW -= hwSensorMarginW;
    hwSensorH -= hwSensorMarginH;

    if (getUsePureBayerReprocessing() == true) {
        ret = getCropRectAlign(hwSensorW, hwSensorH,
                     pictureW, pictureH,
                     &cropX, &cropY,
                     &cropW, &cropH,
                     CAMERA_BCROP_ALIGN, 2,
                     zoomLevel, zoomRatio);

        cropX = ALIGN_DOWN(cropX, 2);
        cropY = ALIGN_DOWN(cropY, 2);
        cropW = hwSensorW - (cropX * 2);
        cropH = hwSensorH - (cropY * 2);

        if (cropW < pictureW / maxZoomRatio || cropH < pictureH / maxZoomRatio) {
            CLOGW(" zoom ratio is upto x%d, crop(%dx%d), picture(%dx%d)", maxZoomRatio, cropW, cropH, pictureW, pictureH);
            cropX = ALIGN_DOWN(((hwSensorW - (pictureW / maxZoomRatio)) >> 1), 2);
            cropY = ALIGN_DOWN(((hwSensorH - (pictureH / maxZoomRatio)) >> 1), 2);
            cropW = hwSensorW - (cropX * 2);
            cropH = hwSensorH - (cropY * 2);
        }
    } else {
        zoomLevel = 0;
        zoomRatio = getZoomRatio(zoomLevel) / 1000;
        getHwBayerCropRegion(&hwSensorCropW, &hwSensorCropH, &hwSensorCropX, &hwSensorCropY);

        ret = getCropRectAlign(hwSensorCropW, hwSensorCropH,
                     pictureW, pictureH,
                     &cropX, &cropY,
                     &cropW, &cropH,
                     CAMERA_BCROP_ALIGN, 2,
                     zoomLevel, zoomRatio);

        cropX = ALIGN_DOWN(cropX, 2);
        cropY = ALIGN_DOWN(cropY, 2);
        cropW = hwSensorCropW - (cropX * 2);
        cropH = hwSensorCropH - (cropY * 2);

        if (cropW < pictureW / maxZoomRatio || cropH < pictureH / maxZoomRatio) {
            CLOGW(" zoom ratio is upto x%d, crop(%dx%d), picture(%dx%d)", maxZoomRatio, cropW, cropH, pictureW, pictureH);
            cropX = ALIGN_DOWN(((hwSensorCropW - (pictureW / maxZoomRatio)) >> 1), 2);
            cropY = ALIGN_DOWN(((hwSensorCropH - (pictureH / maxZoomRatio)) >> 1), 2);
            cropW = hwSensorCropW - (cropX * 2);
            cropH = hwSensorCropH - (cropY * 2);
        }
    }

#if 1
    CLOGD("maxSensorSize (%dx%d), hwSensorSize (%dx%d), previewSize (%dx%d)",
            maxSensorW, maxSensorH, hwSensorW, hwSensorH, previewW, previewH);
    CLOGD("hwPictureSize (%dx%d), pictureSize (%dx%d)",
            hwPictureW, hwPictureH, pictureW, pictureH);
    CLOGD("size cropX = %d, cropY = %d, cropW = %d, cropH = %d, zoom = %d",
            cropX, cropY, cropW, cropH, zoomLevel);
    CLOGD("size2 cropX = %d, cropY = %d, cropW = %d, cropH = %d, zoom = %d",
            crop_crop_x, crop_crop_y, crop_crop_w, crop_crop_h, zoomLevel);
    CLOGD("size pictureFormat = 0x%x, JPEG_INPUT_COLOR_FMT = 0x%x",
            pictureFormat, JPEG_INPUT_COLOR_FMT);
#endif

    srcRect->x = 0;
    srcRect->y = 0;
    srcRect->w = maxSensorW;
    srcRect->h = maxSensorH;
    srcRect->fullW = maxSensorW;
    srcRect->fullH = maxSensorH;
    srcRect->colorFormat = bayerFormat;

    dstRect->x = cropX;
    dstRect->y = cropY;
    dstRect->w = cropW;
    dstRect->h = cropH;
    dstRect->fullW = cropW;
    dstRect->fullH = cropH;
    dstRect->colorFormat = bayerFormat;
    return NO_ERROR;
}

status_t ExynosCamera1Parameters::m_getPreviewBdsSize(ExynosRect *dstRect)
{
    int hwBdsW = 0;
    int hwBdsH = 0;
    int videoW = 0, videoH = 0;
    int sizeList[SIZE_LUT_INDEX_END];

    /* matched ratio LUT is not existed, use equation */
    if (m_useSizeTable == false
        || m_staticInfo->previewSizeLut == NULL
        || m_staticInfo->previewSizeLutMax <= m_cameraInfo.previewSizeRatioId
        || m_getPreviewSizeList(sizeList) != NO_ERROR) {
        ExynosRect rect;
        return calcPreviewBDSSize(&rect, dstRect);
    }

    /* use LUT */
    hwBdsW = sizeList[BDS_W];
    hwBdsH = sizeList[BDS_H];

#ifdef SUPPORT_SW_VDIS
    if(isSWVdisMode()) {
        getHwPreviewSize(&videoW, &videoH);
    } else
#endif
#ifdef SAMSUNG_HYPER_MOTION
    if(getHyperMotionMode()) {
        getHwPreviewSize(&videoW, &videoH);
    } else
#endif
    {
        getHwVideoSize(&videoW, &videoH);
    }

    /* Change bds size to video size when large size recording scenario */
    if (getRecordingHint() == true
        && (videoW > hwBdsW || videoH > hwBdsH)) {
        /* Re-calculate bds height when preview ratio != video ratio */
        if (m_cameraInfo.previewSizeRatioId != m_cameraInfo.videoSizeRatioId) {
            CLOGV("preview ratioId(%d) != videoRatioId(%d), use previewRatioId",
                    m_cameraInfo.previewSizeRatioId, m_cameraInfo.videoSizeRatioId);

            /* This function is not use, cause of TPU internal buffer */
            /*
            ExynosRect bnsSize, bcropSize;
            getPreviewBayerCropSize(&bnsSize, &bcropSize);
            hwBdsH = ALIGN_UP(bcropSize.h * videoW / bcropSize.w, 2);

            if (hwBdsH > bcropSize.h) {
                hwBdsW = bcropSize.w;
                hwBdsH = bcropSize.h;
            }
            */
        } else {
            hwBdsW = videoW;
            hwBdsH = videoH;
        }
    }
#ifdef USE_BDS_WIDE_SELFIE
    else if (getShotMode() == SHOT_MODE_FRONT_PANORAMA && !getRecordingHint()) {
        hwBdsW = WIDE_SELFIE_WIDTH;
        hwBdsH = WIDE_SELFIE_HEIGHT;
    }
#endif

    dstRect->x = 0;
    dstRect->y = 0;
    dstRect->w = hwBdsW;
    dstRect->h = hwBdsH;

#ifdef DEBUG_PERFRAME
    CLOGD("hwBdsSize (%dx%d)", dstRect->w, dstRect->h);
#endif

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::calcPreviewBDSSize(ExynosRect *srcRect, ExynosRect *dstRect)
{
    status_t ret = NO_ERROR;
    int previewW = 0, previewH = 0;
    int bayerFormat = getBayerFormat(PIPE_3AA);
    ExynosRect bnsSize;
    ExynosRect bayerCropSize;

    /* Get preview size info */
    getHwPreviewSize(&previewW, &previewH);
    ret = getPreviewBayerCropSize(&bnsSize, &bayerCropSize);
    if (ret != NO_ERROR)
        CLOGE("getPreviewBayerCropSize() failed");

    srcRect->x = bayerCropSize.x;
    srcRect->y = bayerCropSize.y;
    srcRect->w = bayerCropSize.w;
    srcRect->h = bayerCropSize.h;
    srcRect->fullW = bnsSize.w;
    srcRect->fullH = bnsSize.h;
    srcRect->colorFormat = bayerFormat;

    dstRect->x = 0;
    dstRect->y = 0;
    dstRect->w = previewW;
    dstRect->h = previewH;
    dstRect->fullW = previewW;
    dstRect->fullH = previewH;
    dstRect->colorFormat = JPEG_INPUT_COLOR_FMT;

    /* Check the invalid BDS size compared to Bcrop size */
    if (dstRect->w > srcRect->w)
        dstRect->w = srcRect->w;
    if (dstRect->h > srcRect->h)
        dstRect->h = srcRect->h;

#ifdef DEBUG_PERFRAME
    CLOGE("BDS %dx%d Preview %dx%d",
            dstRect->w, dstRect->h, previewW, previewH);
#endif

    return NO_ERROR;
}

status_t ExynosCamera1Parameters::calcPictureBDSSize(ExynosRect *srcRect, ExynosRect *dstRect)
{
    status_t ret = NO_ERROR;
    int pictureW = 0, pictureH = 0;
    int bayerFormat = getBayerFormat(PIPE_3AA_REPROCESSING);
    ExynosRect bnsSize;
    ExynosRect bayerCropSize;

    /* Get picture size info */
    getPictureSize(&pictureW, &pictureH);
    ret = getPictureBayerCropSize(&bnsSize, &bayerCropSize);
    if (ret != NO_ERROR)
        CLOGE("getPictureBayerCropSize() failed");

    srcRect->x = bayerCropSize.x;
    srcRect->y = bayerCropSize.y;
    srcRect->w = bayerCropSize.w;
    srcRect->h = bayerCropSize.h;
    srcRect->fullW = bnsSize.w;
    srcRect->fullH = bnsSize.h;
    srcRect->colorFormat = bayerFormat;

    dstRect->x = 0;
    dstRect->y = 0;
    dstRect->w = pictureW;
    dstRect->h = pictureH;
    dstRect->fullW = pictureW;
    dstRect->fullH = pictureH;
    dstRect->colorFormat = JPEG_INPUT_COLOR_FMT;

    /* Check the invalid BDS size compared to Bcrop size */
    if (dstRect->w > srcRect->w)
        dstRect->w = srcRect->w;
    if (dstRect->h > srcRect->h)
        dstRect->h = srcRect->h;

#ifdef DEBUG_PERFRAME
    CLOGE("BDS %dx%d Picture %dx%d",
            dstRect->w, dstRect->h, pictureW, pictureH);
#endif

    return NO_ERROR;
}

#ifdef SAMSUNG_LBP
bool ExynosCamera1Parameters::getUseBestPic(void)
{
    return m_useBestPic;
}
#endif

bool ExynosCamera1Parameters::doCscRecording(void)
{
    bool ret = true;
    int previewW = 0, previewH = 0;
    int videoW = 0, videoH = 0;
#ifdef USE_LIVE_BROADCAST
    bool plbmode = getPLBMode();
#endif

#ifdef SUPPORT_SW_VDIS
    if(isSWVdisMode()) {
        getHwPreviewSize(&previewW, &previewH);
#ifdef SAMSUNG_DUAL_SOLUTION
        if (getDualCameraMode() == true) {
            ExynosRect fusionSrcRect;
            ExynosRect fusionDstRect;
            if (getFusionSize(previewW, previewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
                CLOGE("getFusionSize(%d, %d) fail", previewW, previewH);
            }

            previewW = fusionDstRect.w;
            previewH = fusionDstRect.h;
		    m_swVDIS_AdjustPreviewSize(&previewW, &previewH);
            CLOGV("In Dual Mode hwPreview size = %d x %d", previewW, previewH);
        } else {
            m_swVDIS_AdjustPreviewSize(&previewW, &previewH);
            CLOGV("No Dual Mode hwPreviewSize = %d x %d", previewW, previewH);
        }
#else
        m_swVDIS_AdjustPreviewSize(&previewW, &previewH);
        CLOGV("hwPreviewSize = %d x %d", previewW, previewH);
#endif

    }
    else
#endif /*SUPPORT_SW_VDIS*/
    {
        getPreviewSize(&previewW, &previewH);
        CLOGV("PreviewSize = %d x %d",   previewW, previewH);
    }

    getVideoSize(&videoW, &videoH);
    CLOGV("videoSize     = %d x %d",   videoW, videoH);

    if (((videoW == 3840 && videoH == 2160) || (videoW == 1920 && videoH == 1080) || (videoW == 2560 && videoH == 1440))
        && m_useAdaptiveCSCRecording == true
        && videoW == previewW
        && videoH == previewH
#ifdef USE_LIVE_BROADCAST
        && plbmode == false
#endif
        && !getFlipHorizontal()
    ) {
        ret = false;
    }

    return ret;
}

#ifdef DEBUG_RAWDUMP
bool ExynosCamera1Parameters::checkBayerDumpEnable(void)
{
    return true;
}
#endif  /* DEBUG_RAWDUMP */

bool ExynosCamera1Parameters::increaseMaxBufferOfPreview(void)
{
    if((getShotMode() == SHOT_MODE_BEAUTY_FACE)||(getShotMode() == SHOT_MODE_FRONT_PANORAMA)
#ifdef LLS_CAPTURE
        || (getLLSOn() == true && (getCameraId() == CAMERA_ID_FRONT || getCameraId() == CAMERA_ID_FRONT_1))
#endif
        || (getVtMode() == 4)
        ) {
        return true;
    } else {
        return false;
    }
}

bool ExynosCamera1Parameters::getSupportedZoomPreviewWIthScaler(void)
{
    bool ret = false;
    //int cameraId = getCameraId();
    //bool flagDual = getDualMode();
    //int fastFpsMode = getFastFpsMode();
    //int vrMode = getVRMode();

#ifdef TEST_GED_HIGH_SPEED_RECORDING
    ret = false;
#else
    /* Exynos8890 preview is not use External scaler */
    /*
    if (cameraId == CAMERA_ID_BACK) {
        if (fastFpsMode > CONFIG_MODE::HIGHSPEED_60 &&
            fastFpsMode < CONFIG_MODE::MAX
            && vrMode != 1) {
            ret = true;
        }
    } else {
        if (flagDual == true) {
            ret = true;
        }
    }
    */
    ret = false;
#endif

    return ret;
}

void ExynosCamera1Parameters::m_setVideoSize(int w, int h)
{
    m_cameraInfo.videoW = w;
    m_cameraInfo.videoH = h;
}

#ifdef USE_FADE_IN_ENTRANCE
status_t ExynosCamera1Parameters::checkFirstEntrance(const CameraParameters& params)
{
    const char *strNewFirstEntrance = params.get("entrance");
    bool curFirstEntrance = getFirstEntrance();
    bool firstEntrance = false;

    CLOGD("[setParameters]newFirstEntrance %s", strNewFirstEntrance);

    if (strNewFirstEntrance != NULL && !strcmp(strNewFirstEntrance, "true")) {
        firstEntrance = true;
    } else {
        firstEntrance = false;
    }

    if (curFirstEntrance != firstEntrance) {
        setFirstEntrance(firstEntrance);
        m_params.set("entrance", strNewFirstEntrance);
    }
    return NO_ERROR;
}

void ExynosCamera1Parameters::setFirstEntrance(bool value)
{
    m_flagFirstEntrance = value;
}

bool ExynosCamera1Parameters::getFirstEntrance(void)
{
    return m_flagFirstEntrance;
}
#endif

#ifdef SAMSUNG_OT
bool ExynosCamera1Parameters::getObjectTrackingEnable(void)
{
    return m_startObjectTracking;
}

bool ExynosCamera1Parameters::getObjectTrackingChanged(void)
{
    return m_objectTrackingAreaChanged;
}

void ExynosCamera1Parameters::setObjectTrackingChanged(bool newValue)
{
    m_objectTrackingAreaChanged = newValue;

    return;
}

int ExynosCamera1Parameters::getObjectTrackingAreas(int* validFocusArea, ExynosRect2* areas, int* weights)
{
    if(m_objectTrackingArea == NULL || m_objectTrackingWeight == NULL) {
        CLOGE(" NULL pointer!");
        return BAD_VALUE;
    }

    *validFocusArea = m_cameraInfo.numValidFocusArea;
    for (int i = 0; i < *validFocusArea; i++) {
        areas[i] = m_objectTrackingArea[i];
        weights[i] = m_objectTrackingWeight[i];
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::setObjectTrackingGet(bool newValue)
{
    m_objectTrackingGet = newValue;

    return;
}

bool ExynosCamera1Parameters::getObjectTrackingGet(void)
{
    return m_objectTrackingGet;
}
#endif

#ifdef SAMSUNG_LBP
void ExynosCamera1Parameters::setNormalBestFrameCount(uint32_t count)
{
    m_normal_best_frame_count = count;
}

uint32_t ExynosCamera1Parameters::getNormalBestFrameCount(void)
{
    return m_normal_best_frame_count;
}

void ExynosCamera1Parameters::resetNormalBestFrameCount(void)
{
    m_normal_best_frame_count = 0;
}

void ExynosCamera1Parameters::setSCPFrameCount(uint32_t count)
{
    m_scp_frame_count = count;
}

uint32_t ExynosCamera1Parameters::getSCPFrameCount(void)
{
    return m_scp_frame_count;
}

void ExynosCamera1Parameters::resetSCPFrameCount(void)
{
    m_scp_frame_count = 0;
}

void ExynosCamera1Parameters::setBayerFrameCount(uint32_t count)
{
    m_bayer_frame_count = count;
}

uint32_t ExynosCamera1Parameters::getBayerFrameCount(void)
{
    return m_bayer_frame_count;
}

void ExynosCamera1Parameters::resetBayerFrameCount(void)
{
    m_bayer_frame_count = 0;
}
#endif

#ifdef SAMSUNG_JQ
void ExynosCamera1Parameters::setJpegQtableStatus(int state)
{
    Mutex::Autolock lock(m_JpegQtableLock);

    m_jpegQtableStatus = state;
}

int ExynosCamera1Parameters::getJpegQtableStatus(void)
{
    Mutex::Autolock lock(m_JpegQtableLock);

    return m_jpegQtableStatus;
}

void ExynosCamera1Parameters::setJpegQtableOn(bool isOn)
{
    Mutex::Autolock lock(m_JpegQtableLock);

    m_isJpegQtableOn = isOn;
}

bool ExynosCamera1Parameters::getJpegQtableOn(void)
{
    Mutex::Autolock lock(m_JpegQtableLock);

    return m_isJpegQtableOn;
}

void ExynosCamera1Parameters::setJpegQtable(unsigned char* qtable)
{
    Mutex::Autolock lock(m_JpegQtableLock);

    memcpy(m_jpegQtable, qtable, sizeof(m_jpegQtable));
}

void ExynosCamera1Parameters::getJpegQtable(unsigned char* qtable)
{
    Mutex::Autolock lock(m_JpegQtableLock);

    memcpy(qtable, m_jpegQtable, sizeof(m_jpegQtable));
}
#endif

#ifdef SAMSUNG_BD
void ExynosCamera1Parameters::setBlurInfo(unsigned char *data, unsigned int size)
{
#ifdef SAMSUNG_UNI_API
    uni_appMarker_add(BD_EXIF_TAG, (char *)data, size, APP_MARKER_5);
#endif
}
#endif

#ifdef SAMSUNG_UTC_TS
void ExynosCamera1Parameters::setUTCInfo()
{
    struct timeval rawtime;
    struct tm timeinfo;
    gettimeofday(&rawtime, NULL);

    gmtime_r((time_t *)&rawtime.tv_sec, &timeinfo);
    struct utc_ts m_utc;
    strftime(m_utc.utc_ts_data, 20, "%Y:%m:%d %H:%M:%S", &timeinfo);
    /* UTC Time Info Tag : 0x1001 */
    char utc_ts_tag[3];
    utc_ts_tag[0] = 0x10;
    utc_ts_tag[1] = 0x01;
    utc_ts_tag[2] = '\0';

#ifdef SAMSUNG_UNI_API
    uni_appMarker_add(utc_ts_tag, m_utc.utc_ts_data, 20, APP_MARKER_5);
#endif
}
#endif

#ifdef SAMSUNG_LLS_DEBLUR
void ExynosCamera1Parameters::setLLSdebugInfo(unsigned char *data, unsigned int size)
{
    char sizeInfo[2] = {(unsigned char)((size >> 8) & 0xFF), (unsigned char)(size & 0xFF)};

    memset(m_staticInfo->lls_exif_info.lls_exif, 0, LLS_EXIF_SIZE);
    memcpy(m_staticInfo->lls_exif_info.lls_exif, LLS_EXIF_TAG, sizeof(LLS_EXIF_TAG));
    memcpy(m_staticInfo->lls_exif_info.lls_exif + sizeof(LLS_EXIF_TAG) - 1, sizeInfo, sizeof(sizeInfo));
    memcpy(m_staticInfo->lls_exif_info.lls_exif + sizeof(LLS_EXIF_TAG) + sizeof(sizeInfo)- 1, data, size);
}
#endif

#ifdef SAMSUNG_LENS_DC
void ExynosCamera1Parameters::setLensDCdebugInfo(unsigned char *data, unsigned int size)
{
    char sizeInfo[2] = {(unsigned char)((size >> 8) & 0xFF), (unsigned char)(size & 0xFF)};

    memset(m_staticInfo->ldc_exif_info.ldc_exif, 0, LDC_EXIF_SIZE);
    memcpy(m_staticInfo->ldc_exif_info.ldc_exif, LDC_EXIF_TAG, sizeof(LDC_EXIF_TAG));
    memcpy(m_staticInfo->ldc_exif_info.ldc_exif + sizeof(LDC_EXIF_TAG) - 1, sizeInfo, sizeof(sizeInfo));
    memcpy(m_staticInfo->ldc_exif_info.ldc_exif + sizeof(LDC_EXIF_TAG) + sizeof(sizeInfo) - 1, data, size);
}
#endif

#ifdef SAMSUNG_STR_CAPTURE
void ExynosCamera1Parameters::setSTRdebugInfo(unsigned char *data, unsigned int size)
{
    char sizeInfo[2] = {(unsigned char)((size >> 8) & 0xFF), (unsigned char)(size & 0xFF)};

    memset(m_staticInfo->str_exif_info.str_exif, 0, STR_EXIF_SIZE);
    memcpy(m_staticInfo->str_exif_info.str_exif, STR_EXIF_TAG, sizeof(STR_EXIF_TAG));
    memcpy(m_staticInfo->str_exif_info.str_exif + sizeof(STR_EXIF_TAG) - 1, sizeInfo, sizeof(sizeInfo));
    memcpy(m_staticInfo->str_exif_info.str_exif + sizeof(STR_EXIF_TAG) + sizeof(sizeInfo) - 1, data, size);
}
#endif

#ifdef SAMSUNG_HRM
void ExynosCamera1Parameters::m_setHRM(int ir_data, int flicker_data, int status)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        m_dualParameters->setDualHRM(ir_data, flicker_data, status);
    else
#endif
    setMetaCtlHRM(&m_metadata, ir_data, flicker_data, status);
}
void ExynosCamera1Parameters::m_setHRM_Hint(bool flag)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        m_dualParameters->setDualHRMHint(flag);
    else
#endif
    m_flagSensorHRM_Hint = flag;
}
bool ExynosCamera1Parameters::m_getHRMHint(void)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        return m_dualParameters->getDualHRMHint();
    else
#endif
    return m_flagSensorHRM_Hint;
}
#endif
#ifdef SAMSUNG_LIGHT_IR
void ExynosCamera1Parameters::m_setLight_IR(SensorListenerEvent_t data)
{
    setMetaCtlLight_IR(&m_metadata, data);
}
void ExynosCamera1Parameters::m_setLight_IR_Hint(bool flag)
{
    m_flagSensorLight_IR_Hint = flag;
}
#endif
#ifdef SAMSUNG_GYRO
void ExynosCamera1Parameters::m_setGyro(SensorListenerEvent_t data)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        m_dualParameters->setDualGyro(data);
    else
#endif
    setMetaCtlGyro(&m_metadata, data);
}
void ExynosCamera1Parameters::m_setGyroHint(bool flag)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        m_dualParameters->setDualGyroHint(flag);
    else
#endif
    m_flagSensorGyroHint = flag;
}
bool ExynosCamera1Parameters::m_getGyroHint(void)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        return m_dualParameters->getDualGyroHint();
    else
#endif
    return m_flagSensorGyroHint;
}
#endif
#ifdef SAMSUNG_ACCELEROMETER
void ExynosCamera1Parameters::m_setAccelerometer(SensorListenerEvent_t data)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        m_dualParameters->setDualAccelerometer(data);
    else
#endif
    setMetaCtlAcceleration(&m_metadata, data);
}
void ExynosCamera1Parameters::m_setAccelerometerHint(bool flag)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        m_dualParameters->setDualAccelerometerHint(flag);
    else
#endif
    m_flagSensorAccelerationHint = flag;
}
bool ExynosCamera1Parameters::m_getAccelerometerHint(void)
{
#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true)
        return m_dualParameters->getDualAccelerometerHint();
    else
#endif
    return m_flagSensorAccelerationHint;
}
#endif

#ifdef FORCE_CAL_RELOAD
bool ExynosCamera1Parameters::m_checkCalibrationDataValid(char *sensor_fw)
{
    bool ret = true;
    char *tok = NULL;
    char *calcheck = NULL;
    char *last;
    char readdata[50];
    FILE *fp = NULL;

    fp = fopen("/sys/class/camera/rear/rear_force_cal_load", "r");
    if (fp == NULL) {
        return ret;
    } else {
        if (fgets(readdata, sizeof(readdata), fp) == NULL) {
            CLOGE("failed to read sysfs entry");
        }
        CLOGD("%s", readdata);

        fclose(fp);
    }

    tok = strtok_r(sensor_fw, " ", &last);
    if (tok != NULL) {
        calcheck = strtok_r(NULL, " ", &last);
    }

    if (calcheck != NULL && strlen(calcheck) >= 2 && !strncmp(calcheck, "NG", 2)) {
        CLOGE("Cal data has error");
        ret = false;
    }

    return ret;
}
#endif

void ExynosCamera1Parameters::setUseCompanion(bool use)
{
    m_use_companion = use;
}

bool ExynosCamera1Parameters::getUseCompanion()
{
    if ((m_cameraId == CAMERA_ID_FRONT && getDualMode() == true) ||
        m_cameraId == CAMERA_ID_BACK_1)
        m_use_companion = false;

    return m_use_companion;
}

#ifdef SUPPORT_DEPTH_MAP
bool ExynosCamera1Parameters::getDepthCallbackOnPreview(void)
{
    return m_flagDepthCallbackOnPreview;
}

void ExynosCamera1Parameters::setDepthCallbackOnPreview(bool enable)
{
    m_flagDepthCallbackOnPreview = enable;
}

bool ExynosCamera1Parameters::getDepthCallbackOnCapture(void)
{
    return m_flagDepthCallbackOnCapture;
}

void ExynosCamera1Parameters::setDepthCallbackOnCapture(bool enable)
{
    m_flagDepthCallbackOnCapture = enable;
}

void ExynosCamera1Parameters::setDisparityMode(enum companion_disparity_mode disparity_mode)
{
    setMetaCtlDisparityMode(&m_metadata, disparity_mode);
}
#endif

void ExynosCamera1Parameters::setIsThumbnailCallbackOn(bool enable)
{
    m_IsThumbnailCallbackOn = enable;
}

bool ExynosCamera1Parameters::getIsThumbnailCallbackOn()
{
    return m_IsThumbnailCallbackOn;
}

void ExynosCamera1Parameters::setHWFCEnable(bool enable)
{
    m_flagHWFCEnable = enable;
}

bool ExynosCamera1Parameters::getHWFCEnable(void)
{
    if (m_flagHWFCEnable && !getOutPutFormatNV21Enable()
#ifdef USE_ODC_CAPTURE
        && !getODCCaptureEnable()
#endif
    ) {
        return true;
    } else {
        return false;
    }
}

bool ExynosCamera1Parameters::getCallbackNeedCSC(void)
{
#ifdef USE_GSC_FOR_PREVIEW
    bool ret = true;

    int previewW = 0, previewH = 0;
    int hwPreviewW = 0, hwPreviewH = 0;
    int previewFormat = getPreviewFormat();

    getPreviewSize(&previewW, &previewH);
    getHwPreviewSize(&hwPreviewW, &hwPreviewH);

#ifdef SAMSUNG_DUAL_SOLUTION
    if (getDualCameraMode() == true) {
        ExynosRect fusionSrcRect;
        ExynosRect fusionDstRect;
        if (getFusionSize(previewW, previewH, &fusionSrcRect, &fusionDstRect) != NO_ERROR) {
            CLOGE("getFusionSize(%d, %d) fail", previewW, previewH);
        }

        hwPreviewW = fusionDstRect.w;
        hwPreviewH = fusionDstRect.h;
    }
#endif

    if ((previewW == hwPreviewW)&&
        (previewH == hwPreviewH)&&
        (previewFormat == V4L2_PIX_FMT_NV21)) {
        ret = false;
    }
#else
    bool ret = false;
#endif

    return ret;
}

bool ExynosCamera1Parameters::getCallbackNeedCopy2Rendering(void)
{
    bool ret = false;
    int previewW = 0, previewH = 0;

#if defined(SUPPORT_SW_VDIS) || defined(SAMSUNG_HYPER_MOTION)
    if (getSWVdisUIMode() == true || getHyperMotionMode() == true) {
        ret = false;
    }
    else
#endif
    {
        getPreviewSize(&previewW, &previewH);
        if (previewW * previewH <= 2224*1080 && getFastFpsMode() != 3) {
            ret = true;
        }
    }

    return ret;
}

#ifdef USE_MULTI_FACING_SINGLE_CAMERA
status_t ExynosCamera1Parameters::checkCameraDirection(const CameraParameters& params)
{
    int newCameraDirection = 0;
    int curCameraDirection = 0;
    const char *strNewCameraDirection = params.get("single-cam");

    if (strNewCameraDirection == NULL) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]strNewCameraDirection %s", strNewCameraDirection);

    if (!strcmp(strNewCameraDirection, "rear"))
        newCameraDirection = 0;
    else if (!strcmp(strNewCameraDirection, "front"))
        newCameraDirection = 1;
    else {
        CLOGE("unmatched single-cam(%s)", strNewCameraDirection);
        return BAD_VALUE;
    }

    curCameraDirection = getCameraDirection();

    if (curCameraDirection != newCameraDirection) {
        m_setCameraDirection(newCameraDirection);
        m_params.set("single-cam", strNewCameraDirection);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setCameraDirection(int cameraDirection)
{
    m_cameraDirection = cameraDirection;
}

int ExynosCamera1Parameters::getCameraDirection(void)
{
    return m_cameraDirection;
}
#endif

bool ExynosCamera1Parameters::getFaceDetectionMode(bool flagCheckingRecording)
{
    bool ret = true;

    /* turn off when dual mode without dual camera(fusion) */
    if (getDualMode() == true
#ifdef USE_DUAL_CAMERA
        && getDualCameraMode() == false
#endif
    )
        ret = false;

    /* turn off when 120/240 fps is set */
    if (getFastFpsMode() == 2 || getFastFpsMode() == 3)
        ret = false;

    /* turn off in case of panorama mode */
    switch (getShotMode()) {
    case SHOT_MODE_3D_PANORAMA:
    case SHOT_MODE_PANORAMA:
        ret = false;
        break;
    default:
        break;
    }

#ifdef USE_LIVE_BROADCAST
    if (getPLBMode() == true)
        ret = false;
#endif

    /* when stopRecording, ignore recording hint */
    if (flagCheckingRecording == true) {
        /* when recording mode, turn off back camera */
        if (getRecordingHint() == true) {
            if (getCameraId() == CAMERA_ID_BACK || getCameraId() == CAMERA_ID_BACK_1)
                ret = false;
        }
    }

#ifdef SAMSUNG_HYPER_MOTION
    if (getHyperMotionMode() == true)
        ret = true;
#endif

    return ret;
}

#ifdef SAMSUNG_QUICK_SWITCH
void ExynosCamera1Parameters::checkQuickSwitchProp()
{
    bool ret = false;
    char propertyValue[PROPERTY_VALUE_MAX];

    property_get("system.camera.CC.disable", propertyValue, "0");
    int quickSwitchValue = atoi(propertyValue);

    if (quickSwitchValue == QUICK_SWITCH_START_BACK
        || quickSwitchValue == QUICK_SWITCH_START_FRONT) {
        ret = true;
    }

    setQuickSwitchFlag(ret);
}

void ExynosCamera1Parameters::setQuickSwitchFlag(bool isQuickSwitch)
{
    m_isQuickSwitch = isQuickSwitch;
}

bool ExynosCamera1Parameters::getQuickSwitchFlag()
{
    if (!m_isQuickSwitch)
        checkQuickSwitchProp();

    return m_isQuickSwitch;
}

void ExynosCamera1Parameters::setQuickSwitchCmd(int cmd)
{
    m_quickSwitchCmd = cmd;
}

int ExynosCamera1Parameters::getQuickSwitchCmd()
{
    return m_quickSwitchCmd;
}
#endif

bool ExynosCamera1Parameters::checkEnablePicture(void)
{
    bool ret = true;
    int fastFpsMode = getFastFpsMode();

    if (getIntelligentMode() == 1
        || getShotMode() == SHOT_MODE_FACE_LOCK
#ifdef SAMSUNG_COLOR_IRIS
        || getShotMode() == SHOT_MODE_COLOR_IRIS
#endif
#ifndef USE_SNAPSHOT_ON_UHD_RECORDING
        || getUHDRecordingMode()
#endif
#ifndef USE_SNAPSHOT_ON_DUAL_RECORDING
        || getEffectRecordingHint()
        || getDualRecordingHint()
#endif
        || (CONFIG_MODE::HIGHSPEED_60 < fastFpsMode && fastFpsMode < CONFIG_MODE::MAX)
    ) {
        ret = false;
    }

    return ret;
}

status_t ExynosCamera1Parameters::checkShutterSpeed(const CameraParameters& params)
{
    int newShutterSpeed = -1;
    int curShutterSpeed = -1;
    int maxShutterSpeed = params.getInt("max-shutterspeed");
    int minShutterSpeed = params.getInt("min-shutterspeed");

    newShutterSpeed = params.getInt("shutterspeed");

    if (newShutterSpeed < minShutterSpeed || newShutterSpeed > maxShutterSpeed) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newShutterSpeed %d", newShutterSpeed);

    curShutterSpeed = getShutterSpeed();

    if (curShutterSpeed != newShutterSpeed) {
        m_setShutterSpeed(newShutterSpeed);
        m_params.set("shutterspeed", newShutterSpeed);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setShutterSpeed(int shutterSpeed)
{
    m_shutterSpeed = shutterSpeed;
}

int ExynosCamera1Parameters::getShutterSpeed(void)
{
    return m_shutterSpeed;
}

status_t ExynosCamera1Parameters::checkGain(const CameraParameters& params)
{
    int newGain = -1;
    int curGain = -1;
    int maxGain = params.getInt("max-gain");
    int minGain = params.getInt("min-gain");

    newGain = params.getInt("gain");

    if (newGain < minGain || newGain > maxGain) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newGain %d", newGain);

    curGain = getGain();

    if (curGain != newGain) {
        m_setGain(newGain);
        m_params.set("gain", newGain);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setGain(int gain)
{
    m_gain = gain;
}

int ExynosCamera1Parameters::getGain(void)
{
    return m_gain;
}

status_t ExynosCamera1Parameters::checkIrLedWidth(const CameraParameters& params)
{
    int newIrLedWidth = -1;
    int curIrLedWidth = -1;
    int maxIrLedWidth = params.getInt("max-pulse-width");
    int minIrLedWidth = params.getInt("min-pulse-width");

    newIrLedWidth = params.getInt("pulse-width");

    if (newIrLedWidth < minIrLedWidth || newIrLedWidth > maxIrLedWidth) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newIrLedWidth %d", newIrLedWidth);

    curIrLedWidth = getIrLedWidth();

    if (curIrLedWidth != newIrLedWidth) {
        m_setIrLedWidth(newIrLedWidth);
        m_params.set("pulse-width", newIrLedWidth);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setIrLedWidth(int irLedWidth)
{
    m_irLedWidth = irLedWidth;
}

int ExynosCamera1Parameters::getIrLedWidth(void)
{
    return m_irLedWidth;
}

status_t ExynosCamera1Parameters::checkIrLedDelay(const CameraParameters& params)
{
    int newIrLedDelay = -1;
    int curIrLedDelay = -1;
    int maxIrLedDelay = params.getInt("max-pulse-delay");
    int minIrLedDelay = params.getInt("min-pulse-delay");

    newIrLedDelay = params.getInt("pulse-delay");

    if (newIrLedDelay < minIrLedDelay || newIrLedDelay > maxIrLedDelay) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newIrLedDelay %d", newIrLedDelay);

    curIrLedDelay = getIrLedDelay();

    if (curIrLedDelay != newIrLedDelay) {
        m_setIrLedDelay(newIrLedDelay);
        m_params.set("pulse-delay", newIrLedDelay);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setIrLedDelay(int irLedDelay)
{
    m_irLedDelay = irLedDelay;
}

int ExynosCamera1Parameters::getIrLedDelay(void)
{
    return m_irLedDelay;
}

status_t ExynosCamera1Parameters::checkIrLedCurrent(const CameraParameters& params)
{
    int newIrLedCurrent = -1;
    int curIrLedCurrent = -1;
    int maxIrLedCurrent = params.getInt("max-dac");
    int minIrLedCurrent = params.getInt("min-dac");

    newIrLedCurrent = params.getInt("dac");

    if (newIrLedCurrent < minIrLedCurrent || newIrLedCurrent > maxIrLedCurrent) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newIrLedCurrent %d", newIrLedCurrent);

    curIrLedCurrent = getIrLedCurrent();

    if (curIrLedCurrent != newIrLedCurrent) {
        m_setIrLedCurrent(newIrLedCurrent);
        m_params.set("dac", newIrLedCurrent);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setIrLedCurrent(int irLedCurrent)
{
    m_irLedCurrent = irLedCurrent;
}

int ExynosCamera1Parameters::getIrLedCurrent(void)
{
    return m_irLedCurrent;
}

status_t ExynosCamera1Parameters::checkIrLedOnTime(const CameraParameters& params)
{
    int newIrLedOnTime = -1;
    int curIrLedOnTime = -1;
    int maxIrLedOnTime = params.getInt("max-led-max-time");
    int minIrLedOnTime = params.getInt("min-led-max-time");
    char propertyValue[PROPERTY_VALUE_MAX];

    newIrLedOnTime = params.getInt("led-max-time");

    property_get("ro.factory.factory_binary", propertyValue, "0");
    if (strncmp(propertyValue, "factory", 7)) {
        return NO_ERROR;
    }

    if (newIrLedOnTime < minIrLedOnTime || newIrLedOnTime > maxIrLedOnTime) {
        return NO_ERROR;
    }

    CLOGD("[setParameters]newIrLedOnTime %d", newIrLedOnTime);

    curIrLedOnTime = getIrLedOnTime();

    if (curIrLedOnTime != newIrLedOnTime) {
        m_setIrLedOnTime(newIrLedOnTime);
        m_params.set("led-max-time", newIrLedOnTime);
    }

    return NO_ERROR;
}

void ExynosCamera1Parameters::m_setIrLedOnTime(int irLedOnTime)
{
    m_irLedOnTime = irLedOnTime;
}

int ExynosCamera1Parameters::getIrLedOnTime(void)
{
    return m_irLedOnTime;
}

#ifdef SAMSUNG_DUAL_SOLUTION
bool ExynosCamera1Parameters::isUseNewCropRect(void)
{
    bool ret = true;
    if (getZoomRatio() == 1.0f || getDualCameraMode() == false ||
        (m_fusionCaptureWrapper->m_getSyncType() == SYNC_TYPE_SYNC && getCameraId() == CAMERA_ID_BACK_1))
        ret = false;
    return ret;
}

void ExynosCamera1Parameters::getDualCropRect(ExynosRect *dualRect)
{
    UTrect *cropROI;
    UNI_PLUGIN_CAMERA_TYPE uniCamType = (UNI_PLUGIN_CAMERA_TYPE)0;

    if (isUseNewCropRect()) {
        if (getCameraId() == CAMERA_ID_BACK)
            uniCamType = UNI_PLUGIN_CAMERA_TYPE_WIDE;
        else
            uniCamType = UNI_PLUGIN_CAMERA_TYPE_TELE;

        cropROI = m_fusionCaptureWrapper->m_getCropROI(uniCamType);
        dualRect->x = cropROI->left;
        dualRect->y = cropROI->top;
        dualRect->w = cropROI->right - cropROI->left;
        dualRect->h = cropROI->bottom - cropROI->top;
    }
}
#endif

}; /* namespace android */
