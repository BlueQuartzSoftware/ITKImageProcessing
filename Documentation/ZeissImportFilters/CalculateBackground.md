CalculateBackground {#calculatebackground}
=====

## Group (Subgroup) ##
ZeissImport (ZeissImport)


## Description ##
This filter takes a series of grayscale images in a given attribute matrix and averages them. The image content only contributes to the average if the value at a given pixel is between the lowest and highest allowed image value set by the user. The background is then fit to a second-order polynomial, and the resulting fit background is calculated and stored in a new array. 

If the user selects Subtract Background from Current Images, the background will be subtracted, and the new images will overwrite the original images in the attribute matrix. 

## Parameters ##
| Name             | Type |
|------------------|------|
| Lowest Allowed Image Value | int |
| Highest Allowed Image Value | int |
| Subtract Background from Current Images | bool |

## Required Attribute Matrix ##

| Type | Default Array Name | Description | Comment |
|------|--------------------|-------------|---------|
| Int  | Tile Attribute Matrix | This attribute matrix should contain the grayscale images, and only the gray scale images over which the background is to be calculated | Currently the only way to generate this attribute matrix is to use the *Zeiss Axio Vision Import* filter.  |


## Created Arrays ##

| Type | Default Array Name | Description | Comment |
|------|--------------------|-------------|---------|
| Float  | BackgroundImage           | This contains the background image (which is the same size as all the images in the attribute matrix)       |    |

## Created Attribute Matrix ##

| Default Name | Description | 
|--------------------|-------------|
| Background | This attribute matrix contains the above created data array |



## Authors: ##

**Copyright:** 2015 BlueQuartz Software, LLC

**Contact Info:** dream3d@bluequartz.net

**Version:** 1.0.0

**License:**  See the License.txt file that came with DREAM3D.


