/*
 * Your License or Copyright Information can go here
 */

#include "ITKRefineTileCoordinates.h"

#include <QtCore/QString>

#include "SIMPLib/DataArrays/StringDataArray.h"
#include "SIMPLib/FilterParameters/AbstractFilterParametersReader.h"
#include "SIMPLib/FilterParameters/AttributeMatrixSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/BooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/FloatFilterParameter.h"
#include "SIMPLib/FilterParameters/IntFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedBooleanFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedChoicesFilterParameter.h"
#include "SIMPLib/FilterParameters/LinkedPathCreationFilterParameter.h"
#include "SIMPLib/FilterParameters/MultiDataContainerSelectionFilterParameter.h"
#include "SIMPLib/FilterParameters/SeparatorFilterParameter.h"
#include "SIMPLib/FilterParameters/StringFilterParameter.h"
#include "SIMPLib/Geometry/ImageGeom.h"

#include "ITKImageProcessing/ITKImageProcessingFilters/util/DetermineStitching.h"

/* Create Enumerations to allow the created Attribute Arrays to take part in renaming */
enum createdPathID : RenameDataPath::DataID_t
{
  AttributeMatrixID21 = 21,

  DataArrayID30 = 30,
  DataArrayID31 = 31,
  DataArrayID32 = 32,
};

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRefineTileCoordinates::ITKRefineTileCoordinates()
: m_DataContainers("")
, m_CommonAttributeMatrixName(ITKImageProcessing::Montage::k_TileAttributeMatrixDefaultName)
, m_CommonDataArrayName(ITKImageProcessing::Montage::k_TileDataArrayDefaultName)
, m_TileOverlap(10.0f)
{
}

// -----------------------------------------------------------------------------
// Class Destructor
// -----------------------------------------------------------------------------
ITKRefineTileCoordinates::~ITKRefineTileCoordinates() = default;

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRefineTileCoordinates::setupFilterParameters()
{
  FilterParameterVectorType parameters;

  parameters.push_back(SeparatorFilterParameter::New("Mosaic Layout", FilterParameter::RequiredArray));
  parameters.push_back(SIMPL_NEW_INT_VEC3_FP("Montage Size (Cols, Rows)", MontageSize, FilterParameter::Parameter, ITKRefineTileCoordinates));
  {
    LinkedChoicesFilterParameter::Pointer parameter = LinkedChoicesFilterParameter::New();
    parameter->setHumanLabel("Import Mode");
    parameter->setPropertyName("ImportMode");
    parameter->setSetterCallback(SIMPL_BIND_SETTER(ITKRefineTileCoordinates, this, ImportMode));
    parameter->setGetterCallback(SIMPL_BIND_GETTER(ITKRefineTileCoordinates, this, ImportMode));
    QVector<QString> choices;
    choices.push_back("Row-By-Row (Comb Order)");
    choices.push_back("Column-By-Column");
    choices.push_back("Snake-By-Row");
    choices.push_back("Snake-By-Column");
    parameter->setChoices(choices);
    QStringList linkedProps;
    parameter->setLinkedProperties(linkedProps);
    parameter->setEditable(false);
    parameter->setCategory(FilterParameter::Parameter);
    parameters.push_back(parameter);
  }

  parameters.push_back(SIMPL_NEW_FLOAT_FP("Tile Overlap (Percent)", TileOverlap, FilterParameter::RequiredArray, ITKRefineTileCoordinates));
  parameters.push_back(SIMPL_NEW_BOOL_FP("Apply Refined Origin to Geometries", ApplyRefinedOrigin, FilterParameter::Parameter, ITKRefineTileCoordinates));

  MultiDataContainerSelectionFilterParameter::RequirementType req;
  req.dcGeometryTypes.push_back(IGeometry::Type::Image);
  req.dcGeometryTypes.push_back(IGeometry::Type::RectGrid);
  parameters.push_back(SeparatorFilterParameter::New("Input Image Setup", FilterParameter::RequiredArray));
  parameters.push_back(SIMPL_NEW_MDC_SELECTION_FP("Select Image Data Containers", DataContainers, FilterParameter::Parameter, ITKRefineTileCoordinates, req));
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Attribute Matrix", CommonAttributeMatrixName, FilterParameter::RequiredArray, ITKRefineTileCoordinates));
  parameters.push_back(SIMPL_NEW_STRING_FP("Common Data Array", CommonDataArrayName, FilterParameter::RequiredArray, ITKRefineTileCoordinates));

  // parameters.push_back(SeparatorFilterParameter::New("Cell Data", FilterParameter::CreatedArray));
  //  parameters.push_back( SIMPL_NEW_AM_WITH_LINKED_DC_FP("Stitched Attribute Matrix", TileCalculatedInfoAttributeMatrixName, AttributeMatrixName,
  //                                     FilterParameter::CreatedArray, ITKRefineTileCoordinates));
  //  parameters.push_back(SIMPL_NEW_DA_WITH_LINKED_AM_FP("Stitched Coordinates", StitchedCoordinatesArrayName, AttributeMatrixName,
  //                                                      TileCalculatedInfoAttributeMatrixName, FilterParameter::CreatedArray,
  //                                                      ITKRefineTileCoordinates));
  //  parameters.push_back(SIMPL_NEW_DA_WITH_LINKED_AM_FP("Stitched Coordinates Names", StitchedArrayNames, AttributeMatrixName,
  //                                                      TileCalculatedInfoAttributeMatrixName, FilterParameter::CreatedArray,
  //                                                      ITKRefineTileCoordinates));

  setFilterParameters(parameters);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRefineTileCoordinates::initialize()
{
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRefineTileCoordinates::ArrayType ITKRefineTileCoordinates::getArrayType()
{
  DataContainerArray::Pointer dca = getDataContainerArray();
  const QVector<size_t> cDims = {1};

  for(const auto& dcName : m_DataContainers)
  {
    DataArrayPath imageArrayPath(dcName, m_CommonAttributeMatrixName, m_CommonDataArrayName);
    if(!getDataContainerArray()->doesAttributeArrayExist(imageArrayPath))
    {
      QString msg = QString("The Attribute Array path that was generated does not point to an existing Attribute Array. Please set the 'Input Attribute Matrix Name' and 'Input Image Array Name' to "
                            "values that will generate a valid path. Current path generated is %2")
                        .arg(imageArrayPath.serialize());
      setErrorCondition(-53000, msg);
      continue;
    }
    AttributeMatrix::Pointer am = dca->getAttributeMatrix(imageArrayPath);

    IDataArray::Pointer da = am->getChildByName(m_CommonDataArrayName);
    if(da->getComponentDimensions() != cDims)
    {
      QString msg;
      QTextStream out(&msg);
      out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not single-component (Grayscale) data. Please select a pattern of AttributeArray Paths that are gray scale images";
      setErrorCondition(-53000, msg);
      return ArrayType::Error;
    }
    QString typeString = da->getTypeAsString();
    if("uint8_t" == typeString)
    {
      return ArrayType::UInt8;
    }
    if("uint16_t" == typeString)
    {
      return ArrayType::UInt16;
    }
    if("float" == typeString)
    {
      return ArrayType::Float32;
    }

    QString msg;
    QTextStream out(&msg);
    out << "Attribute Array Path: " << imageArrayPath.serialize() << " is not of the appropriate type. Please select a pattern of AttributeArray Paths that are gray scale images";
    setErrorCondition(-53004, msg);
    return ArrayType::Error;
  }
  return ArrayType::Error;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
ITKRefineTileCoordinates::GeomType ITKRefineTileCoordinates::getGeomType()
{
  DataContainerArray::Pointer dca = getDataContainerArray();
  for(const auto& dcName : m_DataContainers)
  {
    IGeometryGrid::Pointer gridGeom = dca->getDataContainer(dcName)->getGeometryAs<IGeometryGrid>();
    if(nullptr == gridGeom)
    {
      QString msg;
      QTextStream out(&msg);
      out << "DataContainer: " << dcName << " needs to have an IGeometryGrid assigned. There is either no geometry assign to the Data Container or the Geometry is not of type IGeometryGrid.";
      setErrorCondition(-53001, msg);
      return GeomType::Error;
    }
    switch(gridGeom->getGeometryType())
    {
    case IGeometry::Type::Image:
      return GeomType::ImageGeom;
    case IGeometry::Type::RectGrid:
      return GeomType::RectGridGeom;
    default:
      setErrorCondition(-53002, "Invalid Geometry type detected.  An ImageGeometry or RectGridGeometry is required for incoming data.");
      return GeomType::Error;
    }
  }
  return GeomType::Error;
}

// -----------------------------------------------------------------------------
// This runs to make sure all of the data you have inputed is at least valid. This information is shown in the current structure tab
// -----------------------------------------------------------------------------
void ITKRefineTileCoordinates::dataCheck()
{
  clearErrorCode();
  clearWarningCode();
  initialize();

  // Check for empty list. If list is empty then the OutputGeometry was never formed and it wont help to go on..
  if(m_DataContainers.isEmpty())
  {
    setErrorCondition(-53006, "At least one DataContainer must be selected.");
    return;
  }

  if(m_CommonAttributeMatrixName.isEmpty())
  {
    setErrorCondition(-53007, "The 'Input Attribute Matrix Name' must contain a valid name. Each Data Container should contain an AttributeMatrix of type Cell with this name.");
    return;
  }

  if(m_CommonDataArrayName.isEmpty())
  {
    setErrorCondition(-53008, "The 'Input Image Array Name' must contain a valid name. The Attribute Matrix should have an Attribute Array of the given name.");
    return;
  }

  DataContainerArray::Pointer dca = getDataContainerArray();
  QVector<size_t> cDims = {1};

  // CheckInputArrays() templated on array and geometry types.
  ArrayType arrayType = getArrayType();
  if(arrayType == ITKRefineTileCoordinates::ArrayType::Error)
  {
    return;
  }
  GeomType geomType = getGeomType();
  if(geomType == ITKRefineTileCoordinates::GeomType::Error)
  {
    return;
  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRefineTileCoordinates::preflight()
{
  // These are the REQUIRED lines of CODE to make sure the filter behaves correctly
  setInPreflight(true);              // Set the fact that we are preflighting.
  emit preflightAboutToExecute();    // Emit this signal so that other widgets can do one file update
  emit updateFilterParameters(this); // Emit this signal to have the widgets push their values down to the filter
  dataCheck();                       // Run our DataCheck to make sure everthing is setup correctly
  emit preflightExecuted();          // We are done preflighting this filter
  setInPreflight(false);             // Inform the system this filter is NOT in preflight mode anymore.
}

// -----------------------------------------------------------------------------
template <typename T>
void executeRefinement(ITKRefineTileCoordinates* filter)
{
  DataContainerArray& dca = *(filter->getDataContainerArray());
  ImageGeom::Pointer imageGeom;
  using DataArrayType = DataArray<T>;
  using DataArrayPointerType = typename DataArrayType::Pointer;

  std::vector<DataArrayPointerType> pointers;
  QStringList dataContainerNames = filter->getDataContainers();
  // Loop over each Data Container and get the Input Image Data
  for(const auto& dcName : dataContainerNames)
  {
    DataContainer::Pointer dc = dca.getDataContainer(dcName);
    imageGeom = dc->getGeometryAs<ImageGeom>();

    AttributeMatrix::Pointer am = dc->getAttributeMatrix(filter->getCommonAttributeMatrixName());
    IDataArray::Pointer iData = am->getAttributeArray(filter->getCommonDataArrayName());

    DataArrayPointerType imageData = std::dynamic_pointer_cast<DataArrayType>(iData);
    if(nullptr == imageData)
    {
      filter->setErrorCondition(-58000, "Image Data is not supported.");
      return;
    }
    if(imageData->getNumberOfComponents() != 1)
    {
      filter->setErrorCondition(-58000, "Image Data has more than 1 component");
      return;
    }
    pointers.push_back(imageData);
  }

  SizeVec3Type udims = imageGeom->getDimensions();
  FloatVec3Type origin; // Default constructor gives us 0,0,0
  FloatVec3Type spacing = imageGeom->getSpacing();
  // Otherwise, we're not using the zeiss data method so call this and let everything work itself out
  IntVec3Type tileDims = filter->getMontageSize();
  FloatArrayType::Pointer coordsPtr =
      DetermineStitching::FindGlobalOrigins<T, 3>(filter, tileDims[0], tileDims[1], filter->getImportMode(), filter->getTileOverlap(), pointers, udims, origin, spacing, dataContainerNames);

  // Check for cancel...
  if(filter->getCancel())
  {
    return;
  }

  coordsPtr->setName("Refined Origins");
  FloatArrayType& coords = *coordsPtr;

  using MultiCompType = FloatArrayType;
  // using CompDimsType = MultiCompType::comp_dims_type;
  using MultiCompTypeIterator = MultiCompType::tuple_iterator;
  MultiCompTypeIterator begin = coords.begin<MultiCompTypeIterator>();
  std::cout << "# Define the number of dimensions we are working on\n";
  std::cout << "# Generated in the ITKRefineTileCoordinates filter as part of a debugging aid\n";
  std::cout << "\n";
  std::cout << "dim = 2\n";
  std::cout << "# Define the image coordinates\n";

  for(MultiCompTypeIterator coordIter = begin; coordIter != coords.end<MultiCompTypeIterator>(); coordIter++)
  {
    std::cout << "ImageFileName.XXX; ; (" << coordIter.comp_value(0) << ", " << coordIter.comp_value(1) << ")" << std::endl;
  }

  DataContainer::Pointer dc = DataContainer::New("Tile Origins");
  QVector<size_t> tDims = {coordsPtr->getNumberOfTuples()};
  AttributeMatrix::Pointer resultCoords = AttributeMatrix::New(tDims, "Coordinates", AttributeMatrix::Type::Generic);
  dc->addOrReplaceAttributeMatrix(resultCoords);
  resultCoords->addOrReplaceAttributeArray(coordsPtr);

  if(filter->getApplyRefinedOrigin())
  {
    float* refinedCoordsPtr = coordsPtr->getPointer(0);
    FloatArrayType::Pointer originalCoords = std::dynamic_pointer_cast<FloatArrayType>(coordsPtr->deepCopy());
    originalCoords->setName("Original Origins");

    for(int32_t dcIndex = 0; dcIndex < dataContainerNames.size(); dcIndex++)
    {
      DataContainer::Pointer dc = dca.getDataContainer(dataContainerNames.at(dcIndex));
      imageGeom = dc->getGeometryAs<ImageGeom>();
      // Save the original origin coordinates
      FloatVec3Type origin = imageGeom->getOrigin();
      originalCoords->setTuple(static_cast<size_t>(dcIndex), origin.data());

      FloatVec3Type spacing = imageGeom->getSpacing();
      // Set the refined coordinates into the geometry
      // The refined coords are in Pixel Coords so we need to figure out what that corresponds to in the acutal geometry
      origin[0] = refinedCoordsPtr[dcIndex * 2] * spacing[0];
      origin[1] = refinedCoordsPtr[dcIndex * 2 + 1] * spacing[1];

      imageGeom->setOrigin(origin);
    }
    resultCoords->addOrReplaceAttributeArray(originalCoords);
  }
  dca.addOrReplaceDataContainer(dc);
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
void ITKRefineTileCoordinates::execute()
{
  dataCheck();
  if(getErrorCode() < 0)
  {
    return;
  }
  clearErrorCode();
  clearWarningCode();

  ArrayType arrayType = getArrayType();

  switch(arrayType)
  {
  case ArrayType::UInt8:
    executeRefinement<uint8_t>(this);
    break;
  case ArrayType::UInt16:
    executeRefinement<uint16_t>(this);
    break;
  case ArrayType::Float32:
    executeRefinement<float>(this);
    break;
  case ArrayType::Error:
    setErrorCondition(-58000, "Error Refining Tile Coordinates. Input Data Type was not supported.");
  }

  //  temp->copyIntoArray(m_StitchedCoordinatesPtr.lock());

  //  StringDataArray::Pointer arrayNames = m_DataArrayNamesForStitchedCoordinatesPtr.lock();
  //  arrayNames->resizeTuples(m_PointerList.size());

  //  //Create another data array with the list of names of the images in the same order as the returned stitched coordinates
  //  QList<QString> names = attrMat->getAttributeArrayNames();
  //  for(size_t i = 0; i < names.size(); i++ )
  //  {
  //    arrayNames->setValue(i, names[i]);
  //  }
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRefineTileCoordinates::getCompiledLibraryName() const
{
  return ITKImageProcessingConstants::ITKImageProcessingBaseName;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRefineTileCoordinates::getGroupName() const
{
  return SIMPL::FilterGroups::Unsupported;
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRefineTileCoordinates::getHumanLabel() const
{
  return "ITK::Refine Tile Coordinates";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QUuid ITKRefineTileCoordinates::getUuid()
{
  return QUuid("{49b5feb1-ec05-5a26-af25-00053151d944}");
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
const QString ITKRefineTileCoordinates::getSubGroupName() const
{
  return "Misc";
}

// -----------------------------------------------------------------------------
//
// -----------------------------------------------------------------------------
AbstractFilter::Pointer ITKRefineTileCoordinates::newFilterInstance(bool copyFilterParameters) const
{
  ITKRefineTileCoordinates::Pointer filter = ITKRefineTileCoordinates::New();
  if(copyFilterParameters)
  {
    copyFilterParameterInstanceVariables(filter.get());
  }
  return filter;
}
