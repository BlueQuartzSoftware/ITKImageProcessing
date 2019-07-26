/* ============================================================================
 * Copyright (c) 2009-2019 BlueQuartz Software, LLC
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice, this
 * list of conditions and the following disclaimer in the documentation and/or
 * other materials provided with the distribution.
 *
 * Neither the names of any of the BlueQuartz Software contributors
 * may be used to endorse or promote products derived from this software without
 * specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE
 * USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 * ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

/** THIS FILE WAS AUTO-GENERATED FROM THE ZVI_TAGS.TXT FILE WHICH WAS MANUALLY GENERATED FROM
* THE ZVI DOCUMENTATION FILE THAT IS DOWNLOADED FROM CARL ZEISS UNDER LICENSE.
*/

#pragma once

#include <QtCore/QString>

namespace Zeiss {
namespace MetaXML
{

static const int ziScalingTypeMask = 1023;
static const int ziScalingTypePixel = 0;
static const int ziScalingTypeGigameter = 67;
static const int ziScalingTypeMegameter = 68;
static const int ziScalingTypeKilometer = 69;
static const int ziScalingTypeHectometer = 70;
static const int ziScalingTypeDecameter = 71;
static const int ziScalingTypeMeter = 72;
static const int ziScalingTypeDecimeter = 73;
static const int ziScalingTypeCentimeter = 74;
static const int ziScalingTypeMillimeter = 75;
static const int ziScalingTypeMicrometer = 76;
static const int ziScalingTypeNanometer = 77;
static const int ziScalingTypeSecond = 136;
static const int ziScalingTypeMillisecond = 139;
static const int ziScalingTypeMicrosecond = 140;
static const int ziScalingTypeMinute = 145;
static const int ziScalingTypeHour = 146;
static const int ziScalingTypeInch = 81;
static const int ziScalingTypeMil = 84;

const QString k_ScalingTypeMask("ziScalingTypeMask");
const QString k_ScalingTypePixel("ziScalingTypePixel");
const QString k_ScalingTypeGigameter("ziScalingTypeGigameter");
const QString k_ScalingTypeMegameter("ziScalingTypeMegameter");
const QString k_ScalingTypeKilometer("ziScalingTypeKilometer");
const QString k_ScalingTypeHectometer("ziScalingTypeHectometer");
const QString k_ScalingTypeDecameter("ziScalingTypeDecameter");
const QString k_ScalingTypeMeter("ziScalingTypeMeter");
const QString k_ScalingTypeDecimeter("ziScalingTypeDecimeter");
const QString k_ScalingTypeCentimeter("ziScalingTypeCentimeter");
const QString k_ScalingTypeMillimeter("ziScalingTypeMillimeter");
const QString k_ScalingTypeMicrometer("ziScalingTypeMicrometer");
const QString k_ScalingTypeNanometer("ziScalingTypeNanometer");
const QString k_ScalingTypeSecond("ziScalingTypeSecond");
const QString k_ScalingTypeMillisecond("ziScalingTypeMillisecond");
const QString k_ScalingTypeMicrosecond("ziScalingTypeMicrosecond");
const QString k_ScalingTypeMinute("ziScalingTypeMinute");
const QString k_ScalingTypeHour("ziScalingTypeHour");
const QString k_ScalingTypeInch("ziScalingTypeInch");
const QString k_ScalingTypeMil("ziScalingTypeMil");

static const int CompressionId = 222;
static const int DateMappingTableId = 257;
static const int BlackValueId = 258;
static const int WhitevalueId = 259;
static const int ImageDataMappingAutoRangeId = 260;
static const int ImageThumbnailId = 261;
static const int GammaValueId = 262;
static const int ImageOverExposureId = 264;
static const int ImageRelativeTime1Id = 265;
static const int ImageRelativeTime2Id = 266;
static const int ImageRelativeTime3Id = 267;
static const int ImageRelativeTime4Id = 268;
static const int ImageRelativeTimeId = 300;
static const int ImageBaseTimeFirstId = 301;
static const int ImageBaseTime1Id = 301;
static const int ImageBaseTime2Id = 302;
static const int ImageBaseTime3Id = 303;
static const int ImageBaseTime4Id = 304;
static const int ImageWidthPixelId = 515;
static const int ImageHeightPixelId = 516;
static const int ImageCountRawId = 517;
static const int PixelTypeId = 518;
static const int NumberRawImagesId = 519;
static const int ImageSizeId = 520;
static const int CompressionFactorForSaveId = 521;
static const int DocumentSaveFlagsId = 522;
static const int AcquisitionpauseannotationId = 523;
static const int DocumentSubtypeId = 530;
static const int AcquisitionBitDepthId = 531;
static const int Z_StacksinglerepresentativeId = 534;
static const int ScaleFactorForXId = 769;
static const int ScaleUnitforXId = 770;
static const int ScaleWidthId = 771;
static const int ScaleFactorForYId = 772;
static const int ScaleUnitforYId = 773;
static const int ScaleHeightId = 774;
static const int ScaleFactorForZId = 775;
static const int ScaleUnitforZId = 776;
static const int ScaleDepthId = 777;
static const int ScalingParentId = 778;
static const int DateId = 1001;
static const int CodeId = 1002;
static const int SourceId = 1003;
static const int MessageId = 1004;
static const int CameraImageAcquisitionTimeId = 1025;
static const int Eight_bitAcquisitionId = 1026;
static const int CameraBitDepthId = 1027;
static const int MonoReferenceLowId = 1029;
static const int MonoReferenceHighId = 1030;
static const int RedReferenceLowId = 1031;
static const int RedReferenceHighId = 1032;
static const int GreenReferenceLowId = 1033;
static const int GreenReferenceHighId = 1034;
static const int BlueReferenceLowId = 1035;
static const int BlueReferenceHighId = 1036;
static const int FramegrabberNameId = 1041;
static const int CameraId = 1042;
static const int CameraTriggerSignalTypeId = 1044;
static const int CameraTriggerEnableId = 1045;
static const int GrabberTimeoutId = 1046;
static const int MultiChannelEnabledId = 1281;
static const int MultichannelColourId = 1282;
static const int MultichannelWeightId = 1283;
static const int ChannelNameId = 1284;
static const int DocumentInformationGroupId = 1536;
static const int TitleId = 1537;
static const int AuthorId = 1538;
static const int KeywordsId = 1539;
static const int CommentsId = 1540;
static const int SampleIDId = 1541;
static const int SubjectId = 1542;
static const int RevisionNumberId = 1543;
static const int SaveFolderId = 1544;
static const int FileLinkId = 1545;
static const int DocumentTypeId = 1546;
static const int StorageMediaId = 1547;
static const int FileIDId = 1548;
static const int ReferenceId = 1549;
static const int FileDateId = 1550;
static const int FileSizeId = 1551;
static const int FilenameId = 1553;
static const int FileAttributesId = 1554;
static const int ProjectGroupId = 1792;
static const int AcquisitionDateId = 1793;
static const int LastmodifiedbyId = 1794;
static const int UserCompanyId = 1795;
static const int UserCompanyLogoId = 1796;
static const int ImageId = 1797;
static const int UserIDId = 1800;
static const int UserNameId = 1801;
static const int UserCityId = 1802;
static const int UserAddressId = 1803;
static const int UserCountryId = 1804;
static const int UserPhoneId = 1805;
static const int UserFaxId = 1806;
static const int ObjectiveNameId = 2049;
static const int OptovarId = 2050;
static const int ReflectorId = 2051;
static const int CondenserContrastId = 2052;
static const int TransmittedLightFilter1Id = 2053;
static const int TransmittedLightFilter2Id = 2054;
static const int ReflectedLightShutterId = 2055;
static const int CondenserFrontLensId = 2056;
static const int ExcitationFilerNameId = 2057;
static const int TransmittedLightFieldstopApertureId = 2060;
static const int ReflectedLightApertureId = 2061;
static const int CondenserN_A_Id = 2062;
static const int LightPathId = 2063;
static const int HalogenLampOnId = 2064;
static const int HalogenLampModeId = 2065;
static const int HalogenLampVoltageId = 2066;
static const int FluorescenceLampLevelId = 2068;
static const int FluorescenceLampIntensityId = 2069;
static const int LightManagerisEnabledId = 2070;
static const int FocusPositionId = 2072;
static const int StagePositionXId = 2073;
static const int StagePositionYId = 2074;
static const int MicroscopeNameId = 2075;
static const int ObjectiveMagnificationId = 2076;
static const int ObjectiveN_A_Id = 2077;
static const int MicroscopeIlluminationId = 2078;
static const int ExternalShutter1Id = 2079;
static const int ExternalShutter2Id = 2080;
static const int ExternalShutter3Id = 2081;
static const int ExternalFilterWheel1NameId = 2082;
static const int ExternalFilterWheel2NameId = 2083;
static const int ParfocalCorrectionId = 2084;
static const int ExternalShutter4Id = 2086;
static const int ExternalShutter5Id = 2087;
static const int ExternalShutter6Id = 2088;
static const int ExternalFilterWheel3NameId = 2089;
static const int ExternalFilterWheel4NameId = 2090;
static const int ObjectiveTurretPositionId = 2103;
static const int ObjectiveContrastMethodId = 2104;
static const int ObjectiveImmersionTypeId = 2105;
static const int ReflectorPositionId = 2107;
static const int TransmittedLightFilter1PositionId = 2109;
static const int TransmittedLightFilter2PositionId = 2110;
static const int ExcitationFilterPositionId = 2112;
static const int LampMirrorPositionERSETZTDURCH241Id = 2113;
static const int ExternalFilterWheel1PositionId = 2114;
static const int ExternalFilterWheel2PositionId = 2115;
static const int ExternalFilterWheel3PositionId = 2116;
static const int ExternalFilterWheel4PositionId = 2117;
static const int LightmanagerModeId = 2118;
static const int HalogenLampCalibrationId = 2119;
static const int CondenserNAGoSpeedId = 2120;
static const int TransmittedLightFieldstopGoSpeedId = 2121;
static const int OptovarGoSpeedId = 2122;
static const int FocuscalibratedId = 2123;
static const int FocusBasicPositionId = 2124;
static const int FocusPowerId = 2125;
static const int FocusBacklashId = 2126;
static const int FocusMeasurementOriginId = 2127;
static const int FocusMeasurementDistanceId = 2128;
static const int FocusSpeedId = 2129;
static const int FocusGoSpeedId = 2130;
static const int FocusDistanceId = 2131;
static const int FocusInitPositionId = 2132;
static const int StagecalibratedId = 2133;
static const int StagePowerId = 2134;
static const int StageXBacklashId = 2135;
static const int StageYBacklashId = 2136;
static const int StageSpeedXId = 2137;
static const int StageSpeedYId = 2138;
static const int StageSpeedId = 2139;
static const int StageGoSpeedXId = 2140;
static const int StageGoSpeedYId = 2141;
static const int StageStepDistanceXId = 2142;
static const int StageStepDistanceYId = 2143;
static const int StageInitialisationPositionXId = 2144;
static const int StageInitialisationPositionYId = 2145;
static const int MicroscopeMagnificationId = 2146;
static const int ReflectorMagnificationId = 2147;
static const int LampMirrorPositionId = 2148;
static const int FocusDepthId = 2149;
static const int MicroscopeTypeId = 2150;
static const int ObjectiveWorkingDistanceId = 2151;
static const int ReflectedLightApertureGoSpeedId = 2152;
static const int ExternalShutterId = 2153;
static const int ObjectiveImmersionStopId = 2154;
static const int FocusStartSpeedId = 2155;
static const int FocusAccelerationId = 2156;
static const int ReflectedLightFieldstopId = 2157;
static const int ReflectedLightFieldstopGoSpeedId = 2158;
static const int ReflectedLightFilter1Id = 2159;
static const int ReflectedLightFilter2Id = 2160;
static const int ReflectedLightFilter1PositionId = 2161;
static const int ReflectedLightFilter2PositionId = 2162;
static const int TransmittedLightAttenuatorId = 2163;
static const int ReflectedLightAttenuatorId = 2164;
static const int TransmittedLightShutterId = 2165;
static const int TransmittedLightAttenuatorGoSpeedId = 2166;
static const int ReflectedLightAttenuatorGoSpeedId = 2167;
static const int TransmittedLightVirtualFilterPositionId = 2176;
static const int TransmittedLightVirtualFilterId = 2177;
static const int ReflectedLightVirtualFilterPositionId = 2178;
static const int ReflectedLightVirtualFilterId = 2179;
static const int ReflectedLightHalogenLampModeId = 2180;
static const int ReflectedLightHalogenLampVoltageId = 2181;
static const int ReflectedLightHalogenLampColourTemperatureId = 2182;
static const int ContrastmanagerModeId = 2183;
static const int DazzleProtectionActiveId = 2184;
static const int ZoomId = 2195;
static const int ZoomGoSpeedId = 2196;
static const int LightZoomId = 2197;
static const int LightZoomGoSpeedId = 2198;
static const int LightzoomCoupledId = 2199;
static const int TransmittedLightHalogenLampModeId = 2200;
static const int TransmittedLightHalogenLampVoltageId = 2201;
static const int TransmittedLightHalogenLampColourTemperatureId = 2202;
static const int ReflectedColdlightModeId = 2203;
static const int ReflectedColdlightIntensityId = 2204;
static const int ReflectedColdlightColourTemperatureId = 2205;
static const int TransmittedColdlightModeId = 2206;
static const int TransmittedColdlightIntensityId = 2207;
static const int TransmittedColdlightColourTemperatureId = 2208;
static const int InfinityspacePortchangerPositionId = 2209;
static const int BeamsplitterInfinitySpaceId = 2210;
static const int TwoTvVisCamChangerPositionId = 2211;
static const int BeamsplitterOcularId = 2212;
static const int TwoTvCamerasChangerPositionId = 2213;
static const int BeamsplitterCamerasId = 2214;
static const int OcularShutterId = 2215;
static const int TwoTvCamerasChangerCubeId = 2216;
static const int LightWaveLengthId = 2217;
static const int OcularMagnificationId = 2218;
static const int CameraAdapterMagnificationId = 2219;
static const int MicroscopePortId = 2220;
static const int OcularTotalMagnificationId = 2221;
static const int FieldofViewId = 2222;
static const int OcularId = 2223;
static const int CameraAdapterId = 2224;
static const int StageJoystickEnabledId = 2225;
static const int ContrastmanagerContrastMethodId = 2226;
static const int CamerasChangerBeamSplitterTypeId = 2229;
static const int RearportSliderPositionId = 2235;
static const int RearportSourceId = 2236;
static const int BeamsplitterTypeInfinitySpaceId = 2237;
static const int FluorescenceAttenuatorId = 2238;
static const int FluorescenceAttenuatorPositionId = 2239;
static const int CameraFramestartLeftId = 2307;
static const int CameraFramestartTopId = 2308;
static const int CameraFrameWidthId = 2309;
static const int CameraFrameHeightId = 2310;
static const int CameraBinningId = 2311;
static const int CameraFrameFullId = 2312;
static const int CameraFramePixelDistanceId = 2313;
static const int DataFormatUseScalingId = 2318;
static const int CameraFrameImageOrientationId = 2319;
static const int VideoMonochromeSignalTypeId = 2320;
static const int VideoColorSignalTypeId = 2321;
static const int MeteorChannelInputId = 2322;
static const int MeteorChannelSyncId = 2323;
static const int WhiteBalanceEnabledId = 2324;
static const int CameraWhiteBalanceRedId = 2325;
static const int CameraWhiteBalanceGreenId = 2326;
static const int CameraWhiteBalanceBlueId = 2327;
static const int CameraFrameScalingFactorId = 2331;
static const int MeteorCameraTypeId = 2562;
static const int ExposureTime_ms_Id = 2564;
static const int CameraExposureTimeAutoCalculateId = 2568;
static const int MeteorGainValueId = 2569;
static const int MeteorGainAutomaticId = 2571;
static const int MeteorAdjustHueId = 2572;
static const int MeteorAdjustSaturationId = 2573;
static const int MeteorAdjustRedLowId = 2574;
static const int MeteorAdjustGreenLowId = 2575;
static const int MeteorBlueLowId = 2576;
static const int MeteorAdjustRedHighId = 2577;
static const int MeteorAdjustGreenHighId = 2578;
static const int MeteorBlueHighId = 2579;
static const int CameraExposureTimeCalculationControlId = 2582;
static const int AxioCamFadingCorrectionEnableId = 2585;
static const int CameraLiveImageId = 2587;
static const int CameraLiveEnabledId = 2588;
static const int LiveImageSyncObjectNameId = 2589;
static const int CameraLiveSpeedId = 2590;
static const int CameraImageId = 2591;
static const int CameraImageWidthId = 2592;
static const int CameraImageHeightId = 2593;
static const int CameraImagePixelTypeId = 2594;
static const int CameraImageShMemoryNameId = 2595;
static const int CameraLiveImageWidthId = 2596;
static const int CameraLiveImageHeightId = 2597;
static const int CameraLiveImagePixelTypeId = 2598;
static const int CameraLiveImageShMemoryNameId = 2599;
static const int CameraLiveMaximumSpeedId = 2600;
static const int CameraLiveBinningId = 2601;
static const int CameraLiveGainValueId = 2602;
static const int CameraLiveExposureTimeValueId = 2603;
static const int CameraLiveScalingFactorId = 2604;
static const int ImageIndexUId = 2817;
static const int ImageIndexVId = 2818;
static const int ImageIndexZId = 2819;
static const int ImageIndexCId = 2820;
static const int ImageIndexTId = 2821;
static const int ImageTileIndexId = 2822;
static const int ImageacquisitionIndexId = 2823;
static const int ImageCountTilesId = 2824;
static const int ImageCountAId = 2825;
static const int ImageIndexSId = 2827;
static const int ImageIndexRawId = 2828;
static const int ImageCountZId = 2832;
static const int ImageCountCId = 2833;
static const int ImageCountTId = 2834;
static const int ImageCountUId = 2838;
static const int ImageCountVId = 2839;
static const int ImageCountSId = 2840;
static const int OriginalStagePositionXId = 2841;
static const int OriginalStagePositionYId = 2842;
static const int LayerDrawFlagsId = 3088;
static const int RemainingTimeId = 3334;
static const int UserField1Id = 3585;
static const int UserField2Id = 3586;
static const int UserField3Id = 3587;
static const int UserField4Id = 3588;
static const int UserField5Id = 3589;
static const int UserField6Id = 3590;
static const int UserField7Id = 3591;
static const int UserField8Id = 3592;
static const int UserField9Id = 3593;
static const int UserField10Id = 3594;
static const int IDId = 3840;
static const int NameId = 3841;
static const int ValueId = 3842;
static const int PvCamClockingModeId = 5501;
static const int AutofocusStatusReportId = 8193;
static const int AutofocusPositionId = 8194;
static const int AutofocusPositionOffsetId = 8195;
static const int AutofocusEmptyFieldThresholdId = 8196;
static const int AutofocusCalibrationNameId = 8197;
static const int AutofocusCurrentCalibrationItemId = 8198;
static const int CameraFrameFullWidthId = 65537;
static const int CameraFrameFullHeightId = 65538;
static const int AxioCamShutterSignalId = 65541;
static const int AxioCamDelayTimeId = 65542;
static const int AxioCamShutterControlId = 65543;
static const int AxioCamBlackRefIsCalculatedId = 65544;
static const int AxioCamBlackReferenceId = 65545;
static const int CameraShadingCorrectionId = 65547;
static const int AxioCamEnhanceColorId = 65550;
static const int AxioCamNIRModeId = 65551;
static const int CameraShutterCloseDelayId = 65552;
static const int CameraWhiteBalanceAutoCalculateId = 65553;
static const int AxioCamNIRModeAvailableId = 65556;
static const int AxioCamFadingCorrectionAvailableId = 65557;
static const int AxioCamEnhanceColorAvailableId = 65559;
static const int MeteorVideoNormId = 65565;
static const int MeteorAdjustWhiteReferenceId = 65566;
static const int MeteorBlackReferenceId = 65567;
static const int MeteorChannelInputCountMonoId = 65568;
static const int MeteorChannelInputCountRGBId = 65570;
static const int MeteorEnableVCRId = 65571;
static const int MeteorBrightnessId = 65572;
static const int MeteorContrastId = 65573;
static const int AxioCamSelectorId = 65575;
static const int AxioCamTypeId = 65576;
static const int AxioCamInfoId = 65577;
static const int AxioCamResolutionId = 65580;
static const int AxioCamColourModelId = 65581;
static const int AxioCamMicroScanningId = 65582;
static const int AmplificationIndexId = 65585;
static const int DeviceCommandId = 65586;
static const int BeamLocationId = 65587;
static const int ComponentTypeId = 65588;
static const int ControllerTypeId = 65589;
static const int CameraWhiteBalanceCalculationRedPaintId = 65590;
static const int CameraWhiteBalanceCalculationBluePaintId = 65591;
static const int CameraWhiteBalanceSetRedId = 65592;
static const int CameraWhiteBalanceSetGreenId = 65593;
static const int CameraWhiteBalanceSetBlueId = 65594;
static const int CameraWhiteBalanceSetTargetRedId = 65595;
static const int CameraWhiteBalanceSetTargetGreenId = 65596;
static const int CameraWhiteBalanceSetTargetBlueId = 65597;
static const int ApotomeCamCalibrationModeId = 65598;
static const int ApoTomeGridPositionId = 65599;
static const int ApotomeCamScannerPositionId = 65600;
static const int ApoTomeFullPhaseShiftId = 65601;
static const int ApoTomeGridNameId = 65602;
static const int ApoTomeStainingId = 65603;
static const int ApoTomeProcessingModeId = 65604;
static const int ApotomeCamLiveCombineModeId = 65605;
static const int ApoTomeFilterNameId = 65606;
static const int ApotomeFilterStrengthId = 65607;
static const int ApotomeCamFilterHarmonicsId = 65608;
static const int ApoTomeGratingPeriodId = 65609;
static const int ApoTomeAutoShutterUsedId = 65610;
static const int ApotomeCamStatusId = 65611;
static const int ApotomeCamNormalizeId = 65612;
static const int ApotomeCamSettingsManagerId = 65613;
static const int DeepviewCamSupervisorModeId = 65614;
static const int DeepViewProcessingId = 65615;
static const int DeepviewCamFilterNameId = 65616;
static const int DeepviewCamStatusId = 65617;
static const int DeepviewCamSettingsManagerId = 65618;
static const int DeviceScalingNameId = 65619;
static const int CameraShadingIsCalculatedId = 65620;
static const int CameraShadingCalculationNameId = 65621;
static const int CameraShadingAutoCalculateId = 65622;
static const int CameraTriggerAvailableId = 65623;
static const int CameraShutterAvailableId = 65626;
static const int AxioCamShutterMicroScanningEnableId = 65627;
static const int ApotomeCamLiveFocusId = 65628;
static const int DeviceInitStatusId = 65629;
static const int DeviceErrorStatusId = 65630;
static const int ApotomeCamSliderInGridPositionId = 65631;
static const int OrcaNIRModeUsedId = 65632;
static const int OrcaAnalogGainId = 65633;
static const int OrcaAnalogOffsetId = 65634;
static const int OrcaBinningId = 65635;
static const int OrcaBitDepthId = 65636;
static const int ApoTomeAveragingCountId = 65637;
static const int DeepViewDoFId = 65638;
static const int DeepViewEDoFId = 65639;
static const int DeepViewSliderNameId = 65643;
static const int RoperCamGainId = 65644;
static const int RoperCamPixelClockId = 65646;
static const int RoperCamTemperatureId = 65647;
static const int CameraImageMemUnitNamesId = 65648;
static const int ApotomeCamLivePhaseId = 65649;
static const int DualAxioCamAlgorithmTypeId = 65650;
static const int ApotomeCamDecayId = 65651;
static const int ApotomeCamEpsilonId = 65652;
static const int AxioCamHSBufferNumberId = 65653;
static const int AxioCamHSFrameTimeId = 65654;
static const int AxioCamAnalogGainEnableId = 65655;
static const int AxioCamAnalogGainAvailableId = 65656;
static const int ApotomeCamPhaseAnglesId = 65657;
static const int ApotomeCamImageFormatId = 65658;
static const int CameraShadingCountId = 65659;
static const int CameraImageRawSizeId = 65660;
static const int ApotomeCamBurstModeId = 65661;
static const int ApotomeCamGenericCameraNameId = 65662;
static const int AcquisitionDeviceId = 65663;
static const int ApotomeGratingPeriodMeasuredId = 65664;
static const int CameraLutEnableId = 65665;
static const int AxioCamSaturationId = 65666;
static const int CameraColorCorrectionId = 65667;
static const int CameraColorProcessingEnableId = 65668;
static const int CameraAnalogGainId = 65669;
static const int CameraWhiteBalanceTargetPosXId = 65670;
static const int CameraWhiteBalanceTargetPosYId = 65671;
static const int CameraShutterSignalPortId = 65672;
static const int AxioCamICSaturationId = 65673;
static const int ApotomeCamCamCalibModeId = 65674;
static const int ApotomeCamCamCalibValueId = 65675;
static const int ApotomeCamAdminCalibModeId = 65676;
static const int ApotomeCamIsAdminId = 65677;
static const int ApotomeCamPwId = 65678;
static const int ApotomeCamAdminNameId = 65679;
static const int CameraShutterLiveEnableId = 65680;
static const int CameraExposureTimeAutoLiveEnableId = 65681;
static const int CameraEMGainId = 65682;
static const int ApotomeCamHardwareVersionId = 65683;
static const int ApotomeCamGridPositionId = 65684;
static const int ApotomeCamAutoGridId = 65685;
static const int OrcaCamNumberOfScanModeId = 65703;
static const int OrcaCamScanModeId = 65704;
static const int OrcaCamEMCCDModeId = 65705;
static const int OrcaCamEMCCDGainId = 65706;
static const int OrcaCamFastAcqId = 65707;
static const int OrcaCamMinExposureTimeId = 65708;
static const int OrcaCamNumberOfPhotonImagingModeId = 65709;
static const int OrcaCamPhotonImagingModeId = 65710;
static const int OrcaCamDirectEMGainAvailableId = 65711;
static const int OrcaCamDirectEMGainId = 65712;
static const int OrcaCamEMGainProtectionAvailableId = 65713;
static const int OrcaCamEMGainProtectionId = 65714;
static const int CameraEMGainMinimumId = 65716;
static const int CameraEMGainMaximumId = 65717;
static const int CameraEMGainAvailableId = 65718;
static const int CameraEMGainEnabledId = 65719;
static const int YokogawaSynchronizeId = 65736;
static const int YokogawaIsInSyncId = 65737;
static const int YokogawaStatusId = 65738;
static const int YokogawaKeepInSyncId = 65739;
static const int YokogawaIsBusyId = 65740;
static const int YokogawaStopDiscId = 65741;
static const int YokogawaCamExposureTimeId = 65742;
static const int YokogawaModeId = 65743;
static const int YokogawaDepthId = 65744;
static const int YokogawaCamReserved12Id = 65745;
static const int YokogawaCamReserved13Id = 65746;
static const int YokogawaCamReserved14Id = 65747;
static const int YokogawaCamReserved15Id = 65748;
static const int YokogawaCamReserved16Id = 65749;
static const int YokogawaCamReserved17Id = 65750;
static const int YokogawaCamReserved18Id = 65751;
static const int YokogawaCamReserved19Id = 65752;
static const int YokogawaCamReserved20Id = 65753;
static const int AuroxCamStatusId = 65754;
static const int AuroxCamInputModeId = 65755;
static const int AuroxCamLiveModeId = 65756;
static const int AuroxCamCalibrationModeId = 65757;
static const int AuroxCamGenericCameraNameId = 65758;
static const int AuroxCamButtonModeId = 65759;
static const int AuroxCamDepthId = 65760;
static const int AuroxCamCenterId = 65761;
static const int AuroxCamFactorId = 65762;
static const int AuroxCamCreateRegistrationId = 65763;
static const int AuroxCamRegistrationValidId = 65764;
static const int AuroxCamRegistrationErrorId = 65765;
static const int AuroxCamShadingImageModeId = 65766;
static const int AuroxCamShadingImageAvailableId = 65767;
static const int AuroxCamQualityId = 65768;
static const int AuroxCamCutLeftId = 65769;
static const int AuroxCamCutTopId = 65770;
static const int AuroxCamCutRightId = 65771;
static const int AuroxCamCutBottomId = 65772;
static const int AuroxCamMeanId = 65773;
static const int AuroxCamNormalizeId = 65774;
static const int AuroxCamUseShadingId = 65775;
static const int AuroxCamShadingValidId = 65776;
static const int AuroxCamNotificationId = 65777;
static const int AuroxCamCalibrationIDId = 65778;
static const int AuroxCamSimpleCalibModeId = 65779;
static const int AuroxCamCalibrationNameId = 65780;
static const int AuroxCamRes6Id = 65781;
static const int AuroxCamCFactorId = 65781;
static const int AuroxCamRegistrationCenterId = 65782;
static const int AuroxCamCalibrationID2Id = 65783;
static const int AuroxCamAveragingId = 65784;
static const int AuroxCamUniqueIDId = 65785;
static const int AuroxCamAutoNormalizeId = 65786;
static const int AuroxCamReserved3Id = 65787;
static const int AuroxCamReserved4Id = 65788;
static const int AuroxCamReserved5Id = 65789;

static const int Factor_0Id = 20000;
static const int Type_0Id = 20001;
static const int Unit_0Id = 20002;
static const int Origin_0Id = 20003;
static const int Angle_0Id = 20004;

static const int Factor_1Id = 20005;
static const int Type_1Id = 20006;
static const int Unit_1Id = 20007;
static const int Origin_1Id = 20008;
static const int Angle_1Id = 20009;

static const int Factor_2Id = 20010;
static const int Type_2Id = 20011;
static const int Unit_2Id = 20012;
static const int Origin_2Id = 20013;
static const int Angle_2Id = 20014;

const QString Compression("Compression");                                                                   // Gets Mapped to Id 222
const QString DateMappingTable("DateMappingTable");                                                         // Gets Mapped to Id 257
const QString BlackValue("BlackValue");                                                                     // Gets Mapped to Id 258
const QString Whitevalue("Whitevalue");                                                                     // Gets Mapped to Id 259
const QString ImageDataMappingAutoRange("ImageDataMappingAutoRange");                                       // Gets Mapped to Id 260
const QString ImageThumbnail("ImageThumbnail");                                                             // Gets Mapped to Id 261
const QString GammaValue("GammaValue");                                                                     // Gets Mapped to Id 262
const QString ImageOverExposure("ImageOverExposure");                                                       // Gets Mapped to Id 264
const QString ImageRelativeTime1("ImageRelativeTime1");                                                     // Gets Mapped to Id 265
const QString ImageRelativeTime2("ImageRelativeTime2");                                                     // Gets Mapped to Id 266
const QString ImageRelativeTime3("ImageRelativeTime3");                                                     // Gets Mapped to Id 267
const QString ImageRelativeTime4("ImageRelativeTime4");                                                     // Gets Mapped to Id 268
const QString ImageRelativeTime("ImageRelativeTime");                                                       // Gets Mapped to Id 300
const QString ImageBaseTimeFirst("ImageBaseTimeFirst");                                                     // Gets Mapped to Id 301
const QString ImageBaseTime1("ImageBaseTime1");                                                             // Gets Mapped to Id 301
const QString ImageBaseTime2("ImageBaseTime2");                                                             // Gets Mapped to Id 302
const QString ImageBaseTime3("ImageBaseTime3");                                                             // Gets Mapped to Id 303
const QString ImageBaseTime4("ImageBaseTime4");                                                             // Gets Mapped to Id 304
const QString ImageWidthPixel("ImageWidthPixel");                                                           // Gets Mapped to Id 515
const QString ImageHeightPixel("ImageHeightPixel");                                                         // Gets Mapped to Id 516
const QString ImageCountRaw("ImageCountRaw");                                                               // Gets Mapped to Id 517
const QString PixelType("PixelType");                                                                       // Gets Mapped to Id 518
const QString NumberRawImages("NumberRawImages");                                                           // Gets Mapped to Id 519
const QString ImageSize("ImageSize");                                                                       // Gets Mapped to Id 520
const QString CompressionFactorForSave("CompressionFactorForSave");                                         // Gets Mapped to Id 521
const QString DocumentSaveFlags("DocumentSaveFlags");                                                       // Gets Mapped to Id 522
const QString Acquisitionpauseannotation("Acquisitionpauseannotation");                                     // Gets Mapped to Id 523
const QString DocumentSubtype("DocumentSubtype");                                                           // Gets Mapped to Id 530
const QString AcquisitionBitDepth("AcquisitionBitDepth");                                                   // Gets Mapped to Id 531
const QString Z_Stacksinglerepresentative("Z_Stacksinglerepresentative");                                   // Gets Mapped to Id 534
const QString ScaleFactorForX("ScaleFactorForX");                                                           // Gets Mapped to Id 769
const QString ScaleUnitforX("ScaleUnitforX");                                                               // Gets Mapped to Id 770
const QString ScaleWidth("ScaleWidth");                                                                     // Gets Mapped to Id 771
const QString ScaleFactorForY("ScaleFactorForY");                                                           // Gets Mapped to Id 772
const QString ScaleUnitforY("ScaleUnitforY");                                                               // Gets Mapped to Id 773
const QString ScaleHeight("ScaleHeight");                                                                   // Gets Mapped to Id 774
const QString ScaleFactorForZ("ScaleFactorForZ");                                                           // Gets Mapped to Id 775
const QString ScaleUnitforZ("ScaleUnitforZ");                                                               // Gets Mapped to Id 776
const QString ScaleDepth("ScaleDepth");                                                                     // Gets Mapped to Id 777
const QString ScalingParent("ScalingParent");                                                               // Gets Mapped to Id 778
const QString Date("Date");                                                                                 // Gets Mapped to Id 1001
const QString Code("Code");                                                                                 // Gets Mapped to Id 1002
const QString Source("Source");                                                                             // Gets Mapped to Id 1003
const QString Message("Message");                                                                           // Gets Mapped to Id 1004
const QString CameraImageAcquisitionTime("CameraImageAcquisitionTime");                                     // Gets Mapped to Id 1025
const QString Eight_bitAcquisition("Eight_bitAcquisition");                                                 // Gets Mapped to Id 1026
const QString CameraBitDepth("CameraBitDepth");                                                             // Gets Mapped to Id 1027
const QString MonoReferenceLow("MonoReferenceLow");                                                         // Gets Mapped to Id 1029
const QString MonoReferenceHigh("MonoReferenceHigh");                                                       // Gets Mapped to Id 1030
const QString RedReferenceLow("RedReferenceLow");                                                           // Gets Mapped to Id 1031
const QString RedReferenceHigh("RedReferenceHigh");                                                         // Gets Mapped to Id 1032
const QString GreenReferenceLow("GreenReferenceLow");                                                       // Gets Mapped to Id 1033
const QString GreenReferenceHigh("GreenReferenceHigh");                                                     // Gets Mapped to Id 1034
const QString BlueReferenceLow("BlueReferenceLow");                                                         // Gets Mapped to Id 1035
const QString BlueReferenceHigh("BlueReferenceHigh");                                                       // Gets Mapped to Id 1036
const QString FramegrabberName("FramegrabberName");                                                         // Gets Mapped to Id 1041
const QString Camera("Camera");                                                                             // Gets Mapped to Id 1042
const QString CameraTriggerSignalType("CameraTriggerSignalType");                                           // Gets Mapped to Id 1044
const QString CameraTriggerEnable("CameraTriggerEnable");                                                   // Gets Mapped to Id 1045
const QString GrabberTimeout("GrabberTimeout");                                                             // Gets Mapped to Id 1046
const QString MultiChannelEnabled("MultiChannelEnabled");                                                   // Gets Mapped to Id 1281
const QString MultichannelColour("MultichannelColour");                                                     // Gets Mapped to Id 1282
const QString MultichannelWeight("MultichannelWeight");                                                     // Gets Mapped to Id 1283
const QString ChannelName("ChannelName");                                                                   // Gets Mapped to Id 1284
const QString DocumentInformationGroup("DocumentInformationGroup");                                         // Gets Mapped to Id 1536
const QString Title("Title");                                                                               // Gets Mapped to Id 1537
const QString Author("Author");                                                                             // Gets Mapped to Id 1538
const QString Keywords("Keywords");                                                                         // Gets Mapped to Id 1539
const QString Comments("Comments");                                                                         // Gets Mapped to Id 1540
const QString SampleID("SampleID");                                                                         // Gets Mapped to Id 1541
const QString Subject("Subject");                                                                           // Gets Mapped to Id 1542
const QString RevisionNumber("RevisionNumber");                                                             // Gets Mapped to Id 1543
const QString SaveFolder("SaveFolder");                                                                     // Gets Mapped to Id 1544
const QString FileLink("FileLink");                                                                         // Gets Mapped to Id 1545
const QString DocumentType("DocumentType");                                                                 // Gets Mapped to Id 1546
const QString StorageMedia("StorageMedia");                                                                 // Gets Mapped to Id 1547
const QString FileID("FileID");                                                                             // Gets Mapped to Id 1548
const QString Reference("Reference");                                                                       // Gets Mapped to Id 1549
const QString FileDate("FileDate");                                                                         // Gets Mapped to Id 1550
const QString FileSize("FileSize");                                                                         // Gets Mapped to Id 1551
const QString Filename("Filename");                                                                         // Gets Mapped to Id 1553
const QString FileAttributes("FileAttributes");                                                             // Gets Mapped to Id 1554
const QString ProjectGroup("ProjectGroup");                                                                 // Gets Mapped to Id 1792
const QString AcquisitionDate("AcquisitionDate");                                                           // Gets Mapped to Id 1793
const QString Lastmodifiedby("Lastmodifiedby");                                                             // Gets Mapped to Id 1794
const QString UserCompany("UserCompany");                                                                   // Gets Mapped to Id 1795
const QString UserCompanyLogo("UserCompanyLogo");                                                           // Gets Mapped to Id 1796
const QString Image("Image");                                                                               // Gets Mapped to Id 1797
const QString UserID("UserID");                                                                             // Gets Mapped to Id 1800
const QString UserName("UserName");                                                                         // Gets Mapped to Id 1801
const QString UserCity("UserCity");                                                                         // Gets Mapped to Id 1802
const QString UserAddress("UserAddress");                                                                   // Gets Mapped to Id 1803
const QString UserCountry("UserCountry");                                                                   // Gets Mapped to Id 1804
const QString UserPhone("UserPhone");                                                                       // Gets Mapped to Id 1805
const QString UserFax("UserFax");                                                                           // Gets Mapped to Id 1806
const QString ObjectiveName("ObjectiveName");                                                               // Gets Mapped to Id 2049
const QString Optovar("Optovar");                                                                           // Gets Mapped to Id 2050
const QString Reflector("Reflector");                                                                       // Gets Mapped to Id 2051
const QString CondenserContrast("CondenserContrast");                                                       // Gets Mapped to Id 2052
const QString TransmittedLightFilter1("TransmittedLightFilter1");                                           // Gets Mapped to Id 2053
const QString TransmittedLightFilter2("TransmittedLightFilter2");                                           // Gets Mapped to Id 2054
const QString ReflectedLightShutter("ReflectedLightShutter");                                               // Gets Mapped to Id 2055
const QString CondenserFrontLens("CondenserFrontLens");                                                     // Gets Mapped to Id 2056
const QString ExcitationFilerName("ExcitationFilerName");                                                   // Gets Mapped to Id 2057
const QString TransmittedLightFieldstopAperture("TransmittedLightFieldstopAperture");                       // Gets Mapped to Id 2060
const QString ReflectedLightAperture("ReflectedLightAperture");                                             // Gets Mapped to Id 2061
const QString CondenserN_A_("CondenserN_A_");                                                               // Gets Mapped to Id 2062
const QString LightPath("LightPath");                                                                       // Gets Mapped to Id 2063
const QString HalogenLampOn("HalogenLampOn");                                                               // Gets Mapped to Id 2064
const QString HalogenLampMode("HalogenLampMode");                                                           // Gets Mapped to Id 2065
const QString HalogenLampVoltage("HalogenLampVoltage");                                                     // Gets Mapped to Id 2066
const QString FluorescenceLampLevel("FluorescenceLampLevel");                                               // Gets Mapped to Id 2068
const QString FluorescenceLampIntensity("FluorescenceLampIntensity");                                       // Gets Mapped to Id 2069
const QString LightManagerisEnabled("LightManagerisEnabled");                                               // Gets Mapped to Id 2070
const QString FocusPosition("FocusPosition");                                                               // Gets Mapped to Id 2072
const QString StagePositionX("StagePositionX");                                                             // Gets Mapped to Id 2073
const QString StagePositionY("StagePositionY");                                                             // Gets Mapped to Id 2074
const QString MicroscopeName("MicroscopeName");                                                             // Gets Mapped to Id 2075
const QString ObjectiveMagnification("ObjectiveMagnification");                                             // Gets Mapped to Id 2076
const QString ObjectiveN_A_("ObjectiveN_A_");                                                               // Gets Mapped to Id 2077
const QString MicroscopeIllumination("MicroscopeIllumination");                                             // Gets Mapped to Id 2078
const QString ExternalShutter1("ExternalShutter1");                                                         // Gets Mapped to Id 2079
const QString ExternalShutter2("ExternalShutter2");                                                         // Gets Mapped to Id 2080
const QString ExternalShutter3("ExternalShutter3");                                                         // Gets Mapped to Id 2081
const QString ExternalFilterWheel1Name("ExternalFilterWheel1Name");                                         // Gets Mapped to Id 2082
const QString ExternalFilterWheel2Name("ExternalFilterWheel2Name");                                         // Gets Mapped to Id 2083
const QString ParfocalCorrection("ParfocalCorrection");                                                     // Gets Mapped to Id 2084
const QString ExternalShutter4("ExternalShutter4");                                                         // Gets Mapped to Id 2086
const QString ExternalShutter5("ExternalShutter5");                                                         // Gets Mapped to Id 2087
const QString ExternalShutter6("ExternalShutter6");                                                         // Gets Mapped to Id 2088
const QString ExternalFilterWheel3Name("ExternalFilterWheel3Name");                                         // Gets Mapped to Id 2089
const QString ExternalFilterWheel4Name("ExternalFilterWheel4Name");                                         // Gets Mapped to Id 2090
const QString ObjectiveTurretPosition("ObjectiveTurretPosition");                                           // Gets Mapped to Id 2103
const QString ObjectiveContrastMethod("ObjectiveContrastMethod");                                           // Gets Mapped to Id 2104
const QString ObjectiveImmersionType("ObjectiveImmersionType");                                             // Gets Mapped to Id 2105
const QString ReflectorPosition("ReflectorPosition");                                                       // Gets Mapped to Id 2107
const QString TransmittedLightFilter1Position("TransmittedLightFilter1Position");                           // Gets Mapped to Id 2109
const QString TransmittedLightFilter2Position("TransmittedLightFilter2Position");                           // Gets Mapped to Id 2110
const QString ExcitationFilterPosition("ExcitationFilterPosition");                                         // Gets Mapped to Id 2112
const QString LampMirrorPositionERSETZTDURCH241("LampMirrorPositionERSETZTDURCH241");                       // Gets Mapped to Id 2113
const QString ExternalFilterWheel1Position("ExternalFilterWheel1Position");                                 // Gets Mapped to Id 2114
const QString ExternalFilterWheel2Position("ExternalFilterWheel2Position");                                 // Gets Mapped to Id 2115
const QString ExternalFilterWheel3Position("ExternalFilterWheel3Position");                                 // Gets Mapped to Id 2116
const QString ExternalFilterWheel4Position("ExternalFilterWheel4Position");                                 // Gets Mapped to Id 2117
const QString LightmanagerMode("LightmanagerMode");                                                         // Gets Mapped to Id 2118
const QString HalogenLampCalibration("HalogenLampCalibration");                                             // Gets Mapped to Id 2119
const QString CondenserNAGoSpeed("CondenserNAGoSpeed");                                                     // Gets Mapped to Id 2120
const QString TransmittedLightFieldstopGoSpeed("TransmittedLightFieldstopGoSpeed");                         // Gets Mapped to Id 2121
const QString OptovarGoSpeed("OptovarGoSpeed");                                                             // Gets Mapped to Id 2122
const QString Focuscalibrated("Focuscalibrated");                                                           // Gets Mapped to Id 2123
const QString FocusBasicPosition("FocusBasicPosition");                                                     // Gets Mapped to Id 2124
const QString FocusPower("FocusPower");                                                                     // Gets Mapped to Id 2125
const QString FocusBacklash("FocusBacklash");                                                               // Gets Mapped to Id 2126
const QString FocusMeasurementOrigin("FocusMeasurementOrigin");                                             // Gets Mapped to Id 2127
const QString FocusMeasurementDistance("FocusMeasurementDistance");                                         // Gets Mapped to Id 2128
const QString FocusSpeed("FocusSpeed");                                                                     // Gets Mapped to Id 2129
const QString FocusGoSpeed("FocusGoSpeed");                                                                 // Gets Mapped to Id 2130
const QString FocusDistance("FocusDistance");                                                               // Gets Mapped to Id 2131
const QString FocusInitPosition("FocusInitPosition");                                                       // Gets Mapped to Id 2132
const QString Stagecalibrated("Stagecalibrated");                                                           // Gets Mapped to Id 2133
const QString StagePower("StagePower");                                                                     // Gets Mapped to Id 2134
const QString StageXBacklash("StageXBacklash");                                                             // Gets Mapped to Id 2135
const QString StageYBacklash("StageYBacklash");                                                             // Gets Mapped to Id 2136
const QString StageSpeedX("StageSpeedX");                                                                   // Gets Mapped to Id 2137
const QString StageSpeedY("StageSpeedY");                                                                   // Gets Mapped to Id 2138
const QString StageSpeed("StageSpeed");                                                                     // Gets Mapped to Id 2139
const QString StageGoSpeedX("StageGoSpeedX");                                                               // Gets Mapped to Id 2140
const QString StageGoSpeedY("StageGoSpeedY");                                                               // Gets Mapped to Id 2141
const QString StageStepDistanceX("StageStepDistanceX");                                                     // Gets Mapped to Id 2142
const QString StageStepDistanceY("StageStepDistanceY");                                                     // Gets Mapped to Id 2143
const QString StageInitialisationPositionX("StageInitialisationPositionX");                                 // Gets Mapped to Id 2144
const QString StageInitialisationPositionY("StageInitialisationPositionY");                                 // Gets Mapped to Id 2145
const QString MicroscopeMagnification("MicroscopeMagnification");                                           // Gets Mapped to Id 2146
const QString ReflectorMagnification("ReflectorMagnification");                                             // Gets Mapped to Id 2147
const QString LampMirrorPosition("LampMirrorPosition");                                                     // Gets Mapped to Id 2148
const QString FocusDepth("FocusDepth");                                                                     // Gets Mapped to Id 2149
const QString MicroscopeType("MicroscopeType");                                                             // Gets Mapped to Id 2150
const QString ObjectiveWorkingDistance("ObjectiveWorkingDistance");                                         // Gets Mapped to Id 2151
const QString ReflectedLightApertureGoSpeed("ReflectedLightApertureGoSpeed");                               // Gets Mapped to Id 2152
const QString ExternalShutter("ExternalShutter");                                                           // Gets Mapped to Id 2153
const QString ObjectiveImmersionStop("ObjectiveImmersionStop");                                             // Gets Mapped to Id 2154
const QString FocusStartSpeed("FocusStartSpeed");                                                           // Gets Mapped to Id 2155
const QString FocusAcceleration("FocusAcceleration");                                                       // Gets Mapped to Id 2156
const QString ReflectedLightFieldstop("ReflectedLightFieldstop");                                           // Gets Mapped to Id 2157
const QString ReflectedLightFieldstopGoSpeed("ReflectedLightFieldstopGoSpeed");                             // Gets Mapped to Id 2158
const QString ReflectedLightFilter1("ReflectedLightFilter1");                                               // Gets Mapped to Id 2159
const QString ReflectedLightFilter2("ReflectedLightFilter2");                                               // Gets Mapped to Id 2160
const QString ReflectedLightFilter1Position("ReflectedLightFilter1Position");                               // Gets Mapped to Id 2161
const QString ReflectedLightFilter2Position("ReflectedLightFilter2Position");                               // Gets Mapped to Id 2162
const QString TransmittedLightAttenuator("TransmittedLightAttenuator");                                     // Gets Mapped to Id 2163
const QString ReflectedLightAttenuator("ReflectedLightAttenuator");                                         // Gets Mapped to Id 2164
const QString TransmittedLightShutter("TransmittedLightShutter");                                           // Gets Mapped to Id 2165
const QString TransmittedLightAttenuatorGoSpeed("TransmittedLightAttenuatorGoSpeed");                       // Gets Mapped to Id 2166
const QString ReflectedLightAttenuatorGoSpeed("ReflectedLightAttenuatorGoSpeed");                           // Gets Mapped to Id 2167
const QString TransmittedLightVirtualFilterPosition("TransmittedLightVirtualFilterPosition");               // Gets Mapped to Id 2176
const QString TransmittedLightVirtualFilter("TransmittedLightVirtualFilter");                               // Gets Mapped to Id 2177
const QString ReflectedLightVirtualFilterPosition("ReflectedLightVirtualFilterPosition");                   // Gets Mapped to Id 2178
const QString ReflectedLightVirtualFilter("ReflectedLightVirtualFilter");                                   // Gets Mapped to Id 2179
const QString ReflectedLightHalogenLampMode("ReflectedLightHalogenLampMode");                               // Gets Mapped to Id 2180
const QString ReflectedLightHalogenLampVoltage("ReflectedLightHalogenLampVoltage");                         // Gets Mapped to Id 2181
const QString ReflectedLightHalogenLampColourTemperature("ReflectedLightHalogenLampColourTemperature");     // Gets Mapped to Id 2182
const QString ContrastmanagerMode("ContrastmanagerMode");                                                   // Gets Mapped to Id 2183
const QString DazzleProtectionActive("DazzleProtectionActive");                                             // Gets Mapped to Id 2184
const QString Zoom("Zoom");                                                                                 // Gets Mapped to Id 2195
const QString ZoomGoSpeed("ZoomGoSpeed");                                                                   // Gets Mapped to Id 2196
const QString LightZoom("LightZoom");                                                                       // Gets Mapped to Id 2197
const QString LightZoomGoSpeed("LightZoomGoSpeed");                                                         // Gets Mapped to Id 2198
const QString LightzoomCoupled("LightzoomCoupled");                                                         // Gets Mapped to Id 2199
const QString TransmittedLightHalogenLampMode("TransmittedLightHalogenLampMode");                           // Gets Mapped to Id 2200
const QString TransmittedLightHalogenLampVoltage("TransmittedLightHalogenLampVoltage");                     // Gets Mapped to Id 2201
const QString TransmittedLightHalogenLampColourTemperature("TransmittedLightHalogenLampColourTemperature"); // Gets Mapped to Id 2202
const QString ReflectedColdlightMode("ReflectedColdlightMode");                                             // Gets Mapped to Id 2203
const QString ReflectedColdlightIntensity("ReflectedColdlightIntensity");                                   // Gets Mapped to Id 2204
const QString ReflectedColdlightColourTemperature("ReflectedColdlightColourTemperature");                   // Gets Mapped to Id 2205
const QString TransmittedColdlightMode("TransmittedColdlightMode");                                         // Gets Mapped to Id 2206
const QString TransmittedColdlightIntensity("TransmittedColdlightIntensity");                               // Gets Mapped to Id 2207
const QString TransmittedColdlightColourTemperature("TransmittedColdlightColourTemperature");               // Gets Mapped to Id 2208
const QString InfinityspacePortchangerPosition("InfinityspacePortchangerPosition");                         // Gets Mapped to Id 2209
const QString BeamsplitterInfinitySpace("BeamsplitterInfinitySpace");                                       // Gets Mapped to Id 2210
const QString TwoTvVisCamChangerPosition("TwoTvVisCamChangerPosition");                                     // Gets Mapped to Id 2211
const QString BeamsplitterOcular("BeamsplitterOcular");                                                     // Gets Mapped to Id 2212
const QString TwoTvCamerasChangerPosition("TwoTvCamerasChangerPosition");                                   // Gets Mapped to Id 2213
const QString BeamsplitterCameras("BeamsplitterCameras");                                                   // Gets Mapped to Id 2214
const QString OcularShutter("OcularShutter");                                                               // Gets Mapped to Id 2215
const QString TwoTvCamerasChangerCube("TwoTvCamerasChangerCube");                                           // Gets Mapped to Id 2216
const QString LightWaveLength("LightWaveLength");                                                           // Gets Mapped to Id 2217
const QString OcularMagnification("OcularMagnification");                                                   // Gets Mapped to Id 2218
const QString CameraAdapterMagnification("CameraAdapterMagnification");                                     // Gets Mapped to Id 2219
const QString MicroscopePort("MicroscopePort");                                                             // Gets Mapped to Id 2220
const QString OcularTotalMagnification("OcularTotalMagnification");                                         // Gets Mapped to Id 2221
const QString FieldofView("FieldofView");                                                                   // Gets Mapped to Id 2222
const QString Ocular("Ocular");                                                                             // Gets Mapped to Id 2223
const QString CameraAdapter("CameraAdapter");                                                               // Gets Mapped to Id 2224
const QString StageJoystickEnabled("StageJoystickEnabled");                                                 // Gets Mapped to Id 2225
const QString ContrastmanagerContrastMethod("ContrastmanagerContrastMethod");                               // Gets Mapped to Id 2226
const QString CamerasChangerBeamSplitterType("CamerasChangerBeamSplitterType");                             // Gets Mapped to Id 2229
const QString RearportSliderPosition("RearportSliderPosition");                                             // Gets Mapped to Id 2235
const QString RearportSource("RearportSource");                                                             // Gets Mapped to Id 2236
const QString BeamsplitterTypeInfinitySpace("BeamsplitterTypeInfinitySpace");                               // Gets Mapped to Id 2237
const QString FluorescenceAttenuator("FluorescenceAttenuator");                                             // Gets Mapped to Id 2238
const QString FluorescenceAttenuatorPosition("FluorescenceAttenuatorPosition");                             // Gets Mapped to Id 2239
const QString CameraFramestartLeft("CameraFramestartLeft");                                                 // Gets Mapped to Id 2307
const QString CameraFramestartTop("CameraFramestartTop");                                                   // Gets Mapped to Id 2308
const QString CameraFrameWidth("CameraFrameWidth");                                                         // Gets Mapped to Id 2309
const QString CameraFrameHeight("CameraFrameHeight");                                                       // Gets Mapped to Id 2310
const QString CameraBinning("CameraBinning");                                                               // Gets Mapped to Id 2311
const QString CameraFrameFull("CameraFrameFull");                                                           // Gets Mapped to Id 2312
const QString CameraFramePixelDistance("CameraFramePixelDistance");                                         // Gets Mapped to Id 2313
const QString DataFormatUseScaling("DataFormatUseScaling");                                                 // Gets Mapped to Id 2318
const QString CameraFrameImageOrientation("CameraFrameImageOrientation");                                   // Gets Mapped to Id 2319
const QString VideoMonochromeSignalType("VideoMonochromeSignalType");                                       // Gets Mapped to Id 2320
const QString VideoColorSignalType("VideoColorSignalType");                                                 // Gets Mapped to Id 2321
const QString MeteorChannelInput("MeteorChannelInput");                                                     // Gets Mapped to Id 2322
const QString MeteorChannelSync("MeteorChannelSync");                                                       // Gets Mapped to Id 2323
const QString WhiteBalanceEnabled("WhiteBalanceEnabled");                                                   // Gets Mapped to Id 2324
const QString CameraWhiteBalanceRed("CameraWhiteBalanceRed");                                               // Gets Mapped to Id 2325
const QString CameraWhiteBalanceGreen("CameraWhiteBalanceGreen");                                           // Gets Mapped to Id 2326
const QString CameraWhiteBalanceBlue("CameraWhiteBalanceBlue");                                             // Gets Mapped to Id 2327
const QString CameraFrameScalingFactor("CameraFrameScalingFactor");                                         // Gets Mapped to Id 2331
const QString MeteorCameraType("MeteorCameraType");                                                         // Gets Mapped to Id 2562
const QString ExposureTime_ms_("ExposureTime_ms_");                                                         // Gets Mapped to Id 2564
const QString CameraExposureTimeAutoCalculate("CameraExposureTimeAutoCalculate");                           // Gets Mapped to Id 2568
const QString MeteorGainValue("MeteorGainValue");                                                           // Gets Mapped to Id 2569
const QString MeteorGainAutomatic("MeteorGainAutomatic");                                                   // Gets Mapped to Id 2571
const QString MeteorAdjustHue("MeteorAdjustHue");                                                           // Gets Mapped to Id 2572
const QString MeteorAdjustSaturation("MeteorAdjustSaturation");                                             // Gets Mapped to Id 2573
const QString MeteorAdjustRedLow("MeteorAdjustRedLow");                                                     // Gets Mapped to Id 2574
const QString MeteorAdjustGreenLow("MeteorAdjustGreenLow");                                                 // Gets Mapped to Id 2575
const QString MeteorBlueLow("MeteorBlueLow");                                                               // Gets Mapped to Id 2576
const QString MeteorAdjustRedHigh("MeteorAdjustRedHigh");                                                   // Gets Mapped to Id 2577
const QString MeteorAdjustGreenHigh("MeteorAdjustGreenHigh");                                               // Gets Mapped to Id 2578
const QString MeteorBlueHigh("MeteorBlueHigh");                                                             // Gets Mapped to Id 2579
const QString CameraExposureTimeCalculationControl("CameraExposureTimeCalculationControl");                 // Gets Mapped to Id 2582
const QString AxioCamFadingCorrectionEnable("AxioCamFadingCorrectionEnable");                               // Gets Mapped to Id 2585
const QString CameraLiveImage("CameraLiveImage");                                                           // Gets Mapped to Id 2587
const QString CameraLiveEnabled("CameraLiveEnabled");                                                       // Gets Mapped to Id 2588
const QString LiveImageSyncObjectName("LiveImageSyncObjectName");                                           // Gets Mapped to Id 2589
const QString CameraLiveSpeed("CameraLiveSpeed");                                                           // Gets Mapped to Id 2590
const QString CameraImage("CameraImage");                                                                   // Gets Mapped to Id 2591
const QString CameraImageWidth("CameraImageWidth");                                                         // Gets Mapped to Id 2592
const QString CameraImageHeight("CameraImageHeight");                                                       // Gets Mapped to Id 2593
const QString CameraImagePixelType("CameraImagePixelType");                                                 // Gets Mapped to Id 2594
const QString CameraImageShMemoryName("CameraImageShMemoryName");                                           // Gets Mapped to Id 2595
const QString CameraLiveImageWidth("CameraLiveImageWidth");                                                 // Gets Mapped to Id 2596
const QString CameraLiveImageHeight("CameraLiveImageHeight");                                               // Gets Mapped to Id 2597
const QString CameraLiveImagePixelType("CameraLiveImagePixelType");                                         // Gets Mapped to Id 2598
const QString CameraLiveImageShMemoryName("CameraLiveImageShMemoryName");                                   // Gets Mapped to Id 2599
const QString CameraLiveMaximumSpeed("CameraLiveMaximumSpeed");                                             // Gets Mapped to Id 2600
const QString CameraLiveBinning("CameraLiveBinning");                                                       // Gets Mapped to Id 2601
const QString CameraLiveGainValue("CameraLiveGainValue");                                                   // Gets Mapped to Id 2602
const QString CameraLiveExposureTimeValue("CameraLiveExposureTimeValue");                                   // Gets Mapped to Id 2603
const QString CameraLiveScalingFactor("CameraLiveScalingFactor");                                           // Gets Mapped to Id 2604
const QString ImageIndexU("ImageIndexU");                                                                   // Gets Mapped to Id 2817
const QString ImageIndexV("ImageIndexV");                                                                   // Gets Mapped to Id 2818
const QString ImageIndexZ("ImageIndexZ");                                                                   // Gets Mapped to Id 2819
const QString ImageIndexC("ImageIndexC");                                                                   // Gets Mapped to Id 2820
const QString ImageIndexT("ImageIndexT");                                                                   // Gets Mapped to Id 2821
const QString ImageTileIndex("ImageTileIndex");                                                             // Gets Mapped to Id 2822
const QString ImageacquisitionIndex("ImageacquisitionIndex");                                               // Gets Mapped to Id 2823
const QString ImageCountTiles("ImageCountTiles");                                                           // Gets Mapped to Id 2824
const QString ImageCountA("ImageCountA");                                                                   // Gets Mapped to Id 2825
const QString ImageIndexS("ImageIndexS");                                                                   // Gets Mapped to Id 2827
const QString ImageIndexRaw("ImageIndexRaw");                                                               // Gets Mapped to Id 2828
const QString ImageCountZ("ImageCountZ");                                                                   // Gets Mapped to Id 2832
const QString ImageCountC("ImageCountC");                                                                   // Gets Mapped to Id 2833
const QString ImageCountT("ImageCountT");                                                                   // Gets Mapped to Id 2834
const QString ImageCountU("ImageCountU");                                                                   // Gets Mapped to Id 2838
const QString ImageCountV("ImageCountV");                                                                   // Gets Mapped to Id 2839
const QString ImageCountS("ImageCountS");                                                                   // Gets Mapped to Id 2840
const QString OriginalStagePositionX("OriginalStagePositionX");                                             // Gets Mapped to Id 2841
const QString OriginalStagePositionY("OriginalStagePositionY");                                             // Gets Mapped to Id 2842
const QString LayerDrawFlags("LayerDrawFlags");                                                             // Gets Mapped to Id 3088
const QString RemainingTime("RemainingTime");                                                               // Gets Mapped to Id 3334
const QString UserField1("UserField1");                                                                     // Gets Mapped to Id 3585
const QString UserField2("UserField2");                                                                     // Gets Mapped to Id 3586
const QString UserField3("UserField3");                                                                     // Gets Mapped to Id 3587
const QString UserField4("UserField4");                                                                     // Gets Mapped to Id 3588
const QString UserField5("UserField5");                                                                     // Gets Mapped to Id 3589
const QString UserField6("UserField6");                                                                     // Gets Mapped to Id 3590
const QString UserField7("UserField7");                                                                     // Gets Mapped to Id 3591
const QString UserField8("UserField8");                                                                     // Gets Mapped to Id 3592
const QString UserField9("UserField9");                                                                     // Gets Mapped to Id 3593
const QString UserField10("UserField10");                                                                   // Gets Mapped to Id 3594
const QString ID("ID");                                                                                     // Gets Mapped to Id 3840
const QString Name("Name");                                                                                 // Gets Mapped to Id 3841
const QString Value("Value");                                                                               // Gets Mapped to Id 3842
const QString PvCamClockingMode("PvCamClockingMode");                                                       // Gets Mapped to Id 5501
const QString AutofocusStatusReport("AutofocusStatusReport");                                               // Gets Mapped to Id 8193
const QString AutofocusPosition("AutofocusPosition");                                                       // Gets Mapped to Id 8194
const QString AutofocusPositionOffset("AutofocusPositionOffset");                                           // Gets Mapped to Id 8195
const QString AutofocusEmptyFieldThreshold("AutofocusEmptyFieldThreshold");                                 // Gets Mapped to Id 8196
const QString AutofocusCalibrationName("AutofocusCalibrationName");                                         // Gets Mapped to Id 8197
const QString AutofocusCurrentCalibrationItem("AutofocusCurrentCalibrationItem");                           // Gets Mapped to Id 8198
const QString CameraFrameFullWidth("CameraFrameFullWidth");                                                 // Gets Mapped to Id 65537
const QString CameraFrameFullHeight("CameraFrameFullHeight");                                               // Gets Mapped to Id 65538
const QString AxioCamShutterSignal("AxioCamShutterSignal");                                                 // Gets Mapped to Id 65541
const QString AxioCamDelayTime("AxioCamDelayTime");                                                         // Gets Mapped to Id 65542
const QString AxioCamShutterControl("AxioCamShutterControl");                                               // Gets Mapped to Id 65543
const QString AxioCamBlackRefIsCalculated("AxioCamBlackRefIsCalculated");                                   // Gets Mapped to Id 65544
const QString AxioCamBlackReference("AxioCamBlackReference");                                               // Gets Mapped to Id 65545
const QString CameraShadingCorrection("CameraShadingCorrection");                                           // Gets Mapped to Id 65547
const QString AxioCamEnhanceColor("AxioCamEnhanceColor");                                                   // Gets Mapped to Id 65550
const QString AxioCamNIRMode("AxioCamNIRMode");                                                             // Gets Mapped to Id 65551
const QString CameraShutterCloseDelay("CameraShutterCloseDelay");                                           // Gets Mapped to Id 65552
const QString CameraWhiteBalanceAutoCalculate("CameraWhiteBalanceAutoCalculate");                           // Gets Mapped to Id 65553
const QString AxioCamNIRModeAvailable("AxioCamNIRModeAvailable");                                           // Gets Mapped to Id 65556
const QString AxioCamFadingCorrectionAvailable("AxioCamFadingCorrectionAvailable");                         // Gets Mapped to Id 65557
const QString AxioCamEnhanceColorAvailable("AxioCamEnhanceColorAvailable");                                 // Gets Mapped to Id 65559
const QString MeteorVideoNorm("MeteorVideoNorm");                                                           // Gets Mapped to Id 65565
const QString MeteorAdjustWhiteReference("MeteorAdjustWhiteReference");                                     // Gets Mapped to Id 65566
const QString MeteorBlackReference("MeteorBlackReference");                                                 // Gets Mapped to Id 65567
const QString MeteorChannelInputCountMono("MeteorChannelInputCountMono");                                   // Gets Mapped to Id 65568
const QString MeteorChannelInputCountRGB("MeteorChannelInputCountRGB");                                     // Gets Mapped to Id 65570
const QString MeteorEnableVCR("MeteorEnableVCR");                                                           // Gets Mapped to Id 65571
const QString MeteorBrightness("MeteorBrightness");                                                         // Gets Mapped to Id 65572
const QString MeteorContrast("MeteorContrast");                                                             // Gets Mapped to Id 65573
const QString AxioCamSelector("AxioCamSelector");                                                           // Gets Mapped to Id 65575
const QString AxioCamType("AxioCamType");                                                                   // Gets Mapped to Id 65576
const QString AxioCamInfo("AxioCamInfo");                                                                   // Gets Mapped to Id 65577
const QString AxioCamResolution("AxioCamResolution");                                                       // Gets Mapped to Id 65580
const QString AxioCamColourModel("AxioCamColourModel");                                                     // Gets Mapped to Id 65581
const QString AxioCamMicroScanning("AxioCamMicroScanning");                                                 // Gets Mapped to Id 65582
const QString AmplificationIndex("AmplificationIndex");                                                     // Gets Mapped to Id 65585
const QString DeviceCommand("DeviceCommand");                                                               // Gets Mapped to Id 65586
const QString BeamLocation("BeamLocation");                                                                 // Gets Mapped to Id 65587
const QString ComponentType("ComponentType");                                                               // Gets Mapped to Id 65588
const QString ControllerType("ControllerType");                                                             // Gets Mapped to Id 65589
const QString CameraWhiteBalanceCalculationRedPaint("CameraWhiteBalanceCalculationRedPaint");               // Gets Mapped to Id 65590
const QString CameraWhiteBalanceCalculationBluePaint("CameraWhiteBalanceCalculationBluePaint");             // Gets Mapped to Id 65591
const QString CameraWhiteBalanceSetRed("CameraWhiteBalanceSetRed");                                         // Gets Mapped to Id 65592
const QString CameraWhiteBalanceSetGreen("CameraWhiteBalanceSetGreen");                                     // Gets Mapped to Id 65593
const QString CameraWhiteBalanceSetBlue("CameraWhiteBalanceSetBlue");                                       // Gets Mapped to Id 65594
const QString CameraWhiteBalanceSetTargetRed("CameraWhiteBalanceSetTargetRed");                             // Gets Mapped to Id 65595
const QString CameraWhiteBalanceSetTargetGreen("CameraWhiteBalanceSetTargetGreen");                         // Gets Mapped to Id 65596
const QString CameraWhiteBalanceSetTargetBlue("CameraWhiteBalanceSetTargetBlue");                           // Gets Mapped to Id 65597
const QString ApotomeCamCalibrationMode("ApotomeCamCalibrationMode");                                       // Gets Mapped to Id 65598
const QString ApoTomeGridPosition("ApoTomeGridPosition");                                                   // Gets Mapped to Id 65599
const QString ApotomeCamScannerPosition("ApotomeCamScannerPosition");                                       // Gets Mapped to Id 65600
const QString ApoTomeFullPhaseShift("ApoTomeFullPhaseShift");                                               // Gets Mapped to Id 65601
const QString ApoTomeGridName("ApoTomeGridName");                                                           // Gets Mapped to Id 65602
const QString ApoTomeStaining("ApoTomeStaining");                                                           // Gets Mapped to Id 65603
const QString ApoTomeProcessingMode("ApoTomeProcessingMode");                                               // Gets Mapped to Id 65604
const QString ApotomeCamLiveCombineMode("ApotomeCamLiveCombineMode");                                       // Gets Mapped to Id 65605
const QString ApoTomeFilterName("ApoTomeFilterName");                                                       // Gets Mapped to Id 65606
const QString ApotomeFilterStrength("ApotomeFilterStrength");                                               // Gets Mapped to Id 65607
const QString ApotomeCamFilterHarmonics("ApotomeCamFilterHarmonics");                                       // Gets Mapped to Id 65608
const QString ApoTomeGratingPeriod("ApoTomeGratingPeriod");                                                 // Gets Mapped to Id 65609
const QString ApoTomeAutoShutterUsed("ApoTomeAutoShutterUsed");                                             // Gets Mapped to Id 65610
const QString ApotomeCamStatus("ApotomeCamStatus");                                                         // Gets Mapped to Id 65611
const QString ApotomeCamNormalize("ApotomeCamNormalize");                                                   // Gets Mapped to Id 65612
const QString ApotomeCamSettingsManager("ApotomeCamSettingsManager");                                       // Gets Mapped to Id 65613
const QString DeepviewCamSupervisorMode("DeepviewCamSupervisorMode");                                       // Gets Mapped to Id 65614
const QString DeepViewProcessing("DeepViewProcessing");                                                     // Gets Mapped to Id 65615
const QString DeepviewCamFilterName("DeepviewCamFilterName");                                               // Gets Mapped to Id 65616
const QString DeepviewCamStatus("DeepviewCamStatus");                                                       // Gets Mapped to Id 65617
const QString DeepviewCamSettingsManager("DeepviewCamSettingsManager");                                     // Gets Mapped to Id 65618
const QString DeviceScalingName("DeviceScalingName");                                                       // Gets Mapped to Id 65619
const QString CameraShadingIsCalculated("CameraShadingIsCalculated");                                       // Gets Mapped to Id 65620
const QString CameraShadingCalculationName("CameraShadingCalculationName");                                 // Gets Mapped to Id 65621
const QString CameraShadingAutoCalculate("CameraShadingAutoCalculate");                                     // Gets Mapped to Id 65622
const QString CameraTriggerAvailable("CameraTriggerAvailable");                                             // Gets Mapped to Id 65623
const QString CameraShutterAvailable("CameraShutterAvailable");                                             // Gets Mapped to Id 65626
const QString AxioCamShutterMicroScanningEnable("AxioCamShutterMicroScanningEnable");                       // Gets Mapped to Id 65627
const QString ApotomeCamLiveFocus("ApotomeCamLiveFocus");                                                   // Gets Mapped to Id 65628
const QString DeviceInitStatus("DeviceInitStatus");                                                         // Gets Mapped to Id 65629
const QString DeviceErrorStatus("DeviceErrorStatus");                                                       // Gets Mapped to Id 65630
const QString ApotomeCamSliderInGridPosition("ApotomeCamSliderInGridPosition");                             // Gets Mapped to Id 65631
const QString OrcaNIRModeUsed("OrcaNIRModeUsed");                                                           // Gets Mapped to Id 65632
const QString OrcaAnalogGain("OrcaAnalogGain");                                                             // Gets Mapped to Id 65633
const QString OrcaAnalogOffset("OrcaAnalogOffset");                                                         // Gets Mapped to Id 65634
const QString OrcaBinning("OrcaBinning");                                                                   // Gets Mapped to Id 65635
const QString OrcaBitDepth("OrcaBitDepth");                                                                 // Gets Mapped to Id 65636
const QString ApoTomeAveragingCount("ApoTomeAveragingCount");                                               // Gets Mapped to Id 65637
const QString DeepViewDoF("DeepViewDoF");                                                                   // Gets Mapped to Id 65638
const QString DeepViewEDoF("DeepViewEDoF");                                                                 // Gets Mapped to Id 65639
const QString DeepViewSliderName("DeepViewSliderName");                                                     // Gets Mapped to Id 65643
const QString RoperCamGain("RoperCamGain");                                                                 // Gets Mapped to Id 65644
const QString RoperCamPixelClock("RoperCamPixelClock");                                                     // Gets Mapped to Id 65646
const QString RoperCamTemperature("RoperCamTemperature");                                                   // Gets Mapped to Id 65647
const QString CameraImageMemUnitNames("CameraImageMemUnitNames");                                           // Gets Mapped to Id 65648
const QString ApotomeCamLivePhase("ApotomeCamLivePhase");                                                   // Gets Mapped to Id 65649
const QString DualAxioCamAlgorithmType("DualAxioCamAlgorithmType");                                         // Gets Mapped to Id 65650
const QString ApotomeCamDecay("ApotomeCamDecay");                                                           // Gets Mapped to Id 65651
const QString ApotomeCamEpsilon("ApotomeCamEpsilon");                                                       // Gets Mapped to Id 65652
const QString AxioCamHSBufferNumber("AxioCamHSBufferNumber");                                               // Gets Mapped to Id 65653
const QString AxioCamHSFrameTime("AxioCamHSFrameTime");                                                     // Gets Mapped to Id 65654
const QString AxioCamAnalogGainEnable("AxioCamAnalogGainEnable");                                           // Gets Mapped to Id 65655
const QString AxioCamAnalogGainAvailable("AxioCamAnalogGainAvailable");                                     // Gets Mapped to Id 65656
const QString ApotomeCamPhaseAngles("ApotomeCamPhaseAngles");                                               // Gets Mapped to Id 65657
const QString ApotomeCamImageFormat("ApotomeCamImageFormat");                                               // Gets Mapped to Id 65658
const QString CameraShadingCount("CameraShadingCount");                                                     // Gets Mapped to Id 65659
const QString CameraImageRawSize("CameraImageRawSize");                                                     // Gets Mapped to Id 65660
const QString ApotomeCamBurstMode("ApotomeCamBurstMode");                                                   // Gets Mapped to Id 65661
const QString ApotomeCamGenericCameraName("ApotomeCamGenericCameraName");                                   // Gets Mapped to Id 65662
const QString AcquisitionDevice("AcquisitionDevice");                                                       // Gets Mapped to Id 65663
const QString ApotomeGratingPeriodMeasured("ApotomeGratingPeriodMeasured");                                 // Gets Mapped to Id 65664
const QString CameraLutEnable("CameraLutEnable");                                                           // Gets Mapped to Id 65665
const QString AxioCamSaturation("AxioCamSaturation");                                                       // Gets Mapped to Id 65666
const QString CameraColorCorrection("CameraColorCorrection");                                               // Gets Mapped to Id 65667
const QString CameraColorProcessingEnable("CameraColorProcessingEnable");                                   // Gets Mapped to Id 65668
const QString CameraAnalogGain("CameraAnalogGain");                                                         // Gets Mapped to Id 65669
const QString CameraWhiteBalanceTargetPosX("CameraWhiteBalanceTargetPosX");                                 // Gets Mapped to Id 65670
const QString CameraWhiteBalanceTargetPosY("CameraWhiteBalanceTargetPosY");                                 // Gets Mapped to Id 65671
const QString CameraShutterSignalPort("CameraShutterSignalPort");                                           // Gets Mapped to Id 65672
const QString AxioCamICSaturation("AxioCamICSaturation");                                                   // Gets Mapped to Id 65673
const QString ApotomeCamCamCalibMode("ApotomeCamCamCalibMode");                                             // Gets Mapped to Id 65674
const QString ApotomeCamCamCalibValue("ApotomeCamCamCalibValue");                                           // Gets Mapped to Id 65675
const QString ApotomeCamAdminCalibMode("ApotomeCamAdminCalibMode");                                         // Gets Mapped to Id 65676
const QString ApotomeCamIsAdmin("ApotomeCamIsAdmin");                                                       // Gets Mapped to Id 65677
const QString ApotomeCamPw("ApotomeCamPw");                                                                 // Gets Mapped to Id 65678
const QString ApotomeCamAdminName("ApotomeCamAdminName");                                                   // Gets Mapped to Id 65679
const QString CameraShutterLiveEnable("CameraShutterLiveEnable");                                           // Gets Mapped to Id 65680
const QString CameraExposureTimeAutoLiveEnable("CameraExposureTimeAutoLiveEnable");                         // Gets Mapped to Id 65681
const QString CameraEMGain("CameraEMGain");                                                                 // Gets Mapped to Id 65682
const QString ApotomeCamHardwareVersion("ApotomeCamHardwareVersion");                                       // Gets Mapped to Id 65683
const QString ApotomeCamGridPosition("ApotomeCamGridPosition");                                             // Gets Mapped to Id 65684
const QString ApotomeCamAutoGrid("ApotomeCamAutoGrid");                                                     // Gets Mapped to Id 65685
const QString OrcaCamNumberOfScanMode("OrcaCamNumberOfScanMode");                                           // Gets Mapped to Id 65703
const QString OrcaCamScanMode("OrcaCamScanMode");                                                           // Gets Mapped to Id 65704
const QString OrcaCamEMCCDMode("OrcaCamEMCCDMode");                                                         // Gets Mapped to Id 65705
const QString OrcaCamEMCCDGain("OrcaCamEMCCDGain");                                                         // Gets Mapped to Id 65706
const QString OrcaCamFastAcq("OrcaCamFastAcq");                                                             // Gets Mapped to Id 65707
const QString OrcaCamMinExposureTime("OrcaCamMinExposureTime");                                             // Gets Mapped to Id 65708
const QString OrcaCamNumberOfPhotonImagingMode("OrcaCamNumberOfPhotonImagingMode");                         // Gets Mapped to Id 65709
const QString OrcaCamPhotonImagingMode("OrcaCamPhotonImagingMode");                                         // Gets Mapped to Id 65710
const QString OrcaCamDirectEMGainAvailable("OrcaCamDirectEMGainAvailable");                                 // Gets Mapped to Id 65711
const QString OrcaCamDirectEMGain("OrcaCamDirectEMGain");                                                   // Gets Mapped to Id 65712
const QString OrcaCamEMGainProtectionAvailable("OrcaCamEMGainProtectionAvailable");                         // Gets Mapped to Id 65713
const QString OrcaCamEMGainProtection("OrcaCamEMGainProtection");                                           // Gets Mapped to Id 65714
const QString CameraEMGainMinimum("CameraEMGainMinimum");                                                   // Gets Mapped to Id 65716
const QString CameraEMGainMaximum("CameraEMGainMaximum");                                                   // Gets Mapped to Id 65717
const QString CameraEMGainAvailable("CameraEMGainAvailable");                                               // Gets Mapped to Id 65718
const QString CameraEMGainEnabled("CameraEMGainEnabled");                                                   // Gets Mapped to Id 65719
const QString YokogawaSynchronize("YokogawaSynchronize");                                                   // Gets Mapped to Id 65736
const QString YokogawaIsInSync("YokogawaIsInSync");                                                         // Gets Mapped to Id 65737
const QString YokogawaStatus("YokogawaStatus");                                                             // Gets Mapped to Id 65738
const QString YokogawaKeepInSync("YokogawaKeepInSync");                                                     // Gets Mapped to Id 65739
const QString YokogawaIsBusy("YokogawaIsBusy");                                                             // Gets Mapped to Id 65740
const QString YokogawaStopDisc("YokogawaStopDisc");                                                         // Gets Mapped to Id 65741
const QString YokogawaCamExposureTime("YokogawaCamExposureTime");                                           // Gets Mapped to Id 65742
const QString YokogawaMode("YokogawaMode");                                                                 // Gets Mapped to Id 65743
const QString YokogawaDepth("YokogawaDepth");                                                               // Gets Mapped to Id 65744
const QString YokogawaCamReserved12("YokogawaCamReserved12");                                               // Gets Mapped to Id 65745
const QString YokogawaCamReserved13("YokogawaCamReserved13");                                               // Gets Mapped to Id 65746
const QString YokogawaCamReserved14("YokogawaCamReserved14");                                               // Gets Mapped to Id 65747
const QString YokogawaCamReserved15("YokogawaCamReserved15");                                               // Gets Mapped to Id 65748
const QString YokogawaCamReserved16("YokogawaCamReserved16");                                               // Gets Mapped to Id 65749
const QString YokogawaCamReserved17("YokogawaCamReserved17");                                               // Gets Mapped to Id 65750
const QString YokogawaCamReserved18("YokogawaCamReserved18");                                               // Gets Mapped to Id 65751
const QString YokogawaCamReserved19("YokogawaCamReserved19");                                               // Gets Mapped to Id 65752
const QString YokogawaCamReserved20("YokogawaCamReserved20");                                               // Gets Mapped to Id 65753
const QString AuroxCamStatus("AuroxCamStatus");                                                             // Gets Mapped to Id 65754
const QString AuroxCamInputMode("AuroxCamInputMode");                                                       // Gets Mapped to Id 65755
const QString AuroxCamLiveMode("AuroxCamLiveMode");                                                         // Gets Mapped to Id 65756
const QString AuroxCamCalibrationMode("AuroxCamCalibrationMode");                                           // Gets Mapped to Id 65757
const QString AuroxCamGenericCameraName("AuroxCamGenericCameraName");                                       // Gets Mapped to Id 65758
const QString AuroxCamButtonMode("AuroxCamButtonMode");                                                     // Gets Mapped to Id 65759
const QString AuroxCamDepth("AuroxCamDepth");                                                               // Gets Mapped to Id 65760
const QString AuroxCamCenter("AuroxCamCenter");                                                             // Gets Mapped to Id 65761
const QString AuroxCamFactor("AuroxCamFactor");                                                             // Gets Mapped to Id 65762
const QString AuroxCamCreateRegistration("AuroxCamCreateRegistration");                                     // Gets Mapped to Id 65763
const QString AuroxCamRegistrationValid("AuroxCamRegistrationValid");                                       // Gets Mapped to Id 65764
const QString AuroxCamRegistrationError("AuroxCamRegistrationError");                                       // Gets Mapped to Id 65765
const QString AuroxCamShadingImageMode("AuroxCamShadingImageMode");                                         // Gets Mapped to Id 65766
const QString AuroxCamShadingImageAvailable("AuroxCamShadingImageAvailable");                               // Gets Mapped to Id 65767
const QString AuroxCamQuality("AuroxCamQuality");                                                           // Gets Mapped to Id 65768
const QString AuroxCamCutLeft("AuroxCamCutLeft");                                                           // Gets Mapped to Id 65769
const QString AuroxCamCutTop("AuroxCamCutTop");                                                             // Gets Mapped to Id 65770
const QString AuroxCamCutRight("AuroxCamCutRight");                                                         // Gets Mapped to Id 65771
const QString AuroxCamCutBottom("AuroxCamCutBottom");                                                       // Gets Mapped to Id 65772
const QString AuroxCamMean("AuroxCamMean");                                                                 // Gets Mapped to Id 65773
const QString AuroxCamNormalize("AuroxCamNormalize");                                                       // Gets Mapped to Id 65774
const QString AuroxCamUseShading("AuroxCamUseShading");                                                     // Gets Mapped to Id 65775
const QString AuroxCamShadingValid("AuroxCamShadingValid");                                                 // Gets Mapped to Id 65776
const QString AuroxCamNotification("AuroxCamNotification");                                                 // Gets Mapped to Id 65777
const QString AuroxCamCalibrationID("AuroxCamCalibrationID");                                               // Gets Mapped to Id 65778
const QString AuroxCamSimpleCalibMode("AuroxCamSimpleCalibMode");                                           // Gets Mapped to Id 65779
const QString AuroxCamCalibrationName("AuroxCamCalibrationName");                                           // Gets Mapped to Id 65780
const QString AuroxCamRes6("AuroxCamRes6");                                                                 // Gets Mapped to Id 65781
const QString AuroxCamCFactor("AuroxCamCFactor");                                                           // Gets Mapped to Id 65781
const QString AuroxCamRegistrationCenter("AuroxCamRegistrationCenter");                                     // Gets Mapped to Id 65782
const QString AuroxCamCalibrationID2("AuroxCamCalibrationID2");                                             // Gets Mapped to Id 65783
const QString AuroxCamAveraging("AuroxCamAveraging");                                                       // Gets Mapped to Id 65784
const QString AuroxCamUniqueID("AuroxCamUniqueID");                                                         // Gets Mapped to Id 65785
const QString AuroxCamAutoNormalize("AuroxCamAutoNormalize");                                               // Gets Mapped to Id 65786
const QString AuroxCamReserved3("AuroxCamReserved3");                                                       // Gets Mapped to Id 65787
const QString AuroxCamReserved4("AuroxCamReserved4");                                                       // Gets Mapped to Id 65788
const QString AuroxCamReserved5("AuroxCamReserved5");                                                       // Gets Mapped to Id 65789

const QString Factor_0("Factor_0");
const QString Type_0("Type_0");
const QString Unit_0("Unit_0");
const QString Origin_0("Origin_0");
const QString Angle_0("Angle_2");

const QString Factor_1("Factor_1");
const QString Type_1("Type_1");
const QString Unit_1("Unit_1");
const QString Origin_1("Origin_1");
const QString Angle_1("Angle_1");

const QString Factor_2("Factor_2");
const QString Type_2("Type_2");
const QString Unit_2("Unit_2");
const QString Origin_2("Origin_2");
const QString Angle_2("Angle_2");

} // namespace MetaXML
}
