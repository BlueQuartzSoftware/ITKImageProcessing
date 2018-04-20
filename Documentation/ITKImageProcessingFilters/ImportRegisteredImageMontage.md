# Import Registered Image Montage #


## Group (Subgroup) ##

ITKImageProcessing (Input)


## Description ##

Imports multiple images for the purpose of montage assembly. These images must be 8 bit greyscale, and have the same X pixel dimensions and the same Y pixel dimensions. Each image is stored in it's own attribute array. This filter allows for the import of a registration file that has the registered XY coordinates for each image.

Utilizes the *itkReadImage* filter.

## Example Registration File ##


    # Define the number of dimensions we are working on
    dim = 2

    # Define the image coordinates
    slice_11.tif; ; (0.0, 0.0)
    slice_12.tif; ; (471.2965233276666, -0.522608066434236)
    slice_13.tif; ; (944.5992124792093, -0.7247005913413109)
    slice_14.tif; ; (1419.6556996952306, -0.9362629861595396)
    slice_15.tif; ; (1894.5906379078629, -1.1529111881475664)
    slice_16.tif; ; (2369.039471380481, -1.9017522091702983)
    slice_17.tif; ; (2843.7532978452855, -2.5246588293146055)


## Parameters ##

| Name             |  Type  |
|------------------|--------|
| Input File List | VectorFileListInfo type |
| Registration File Path | String |
| Origin | float x 3 |
| Resolution | float x 3 |
| Data Container Name | String |
| Cell Attribute Matrix Name | String |
| Data Array Name | String |

## Required DataContainers ##

NONE

## Required Objects ##

NONE

## Created Objects ##

| Kind | Default Name | Type | Component Dimensions | Description |
|------|--------------|------|----------------------|-------------|
| **Data Container** | Data Container Name | N/A | N/A |  |
| ImageGeometry |  | N/A | N/A |  |
| Cell AttributeMatrix |  | N/A | N/A |  |
| Image Data |  | N/A | N/A |  |


## Authors: ##

**Contact Info** dream3d@bluequartz.net

**Version** 1.0.0

**License**  See the License.txt file that came with DREAM3D.

## Example Pipelines ##



## License & Copyright ##

Please see the description file distributed with this plugin.

## DREAM3D Mailing Lists ##

If you need more help with a filter, please consider asking your question on the DREAM3D Users mailing list:
https://groups.google.com/forum/?hl=en#!forum/dream3d-users

