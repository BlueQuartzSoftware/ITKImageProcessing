# Blend #


## Group (Subgroup) ##

Processing (Processing)

## Description ##

This **Filter** uses the itk::AmoebaOptimizer to calculate the dewarp parameters required for dewarping a specified montage.  It does this by performing an itk::FFTConvolutionImageFilter over the overlaps in a montage for each set of parameters checked by the amoeba filter.  The amoeba filter then tries to find the set of parameters that maximizes the sum of the max values from each overlap.  These max values are used to determine how similar the overlapping region is.  Maximizing the summation is done to find the set of dewarp parameters resulting in the best-matching overlaps.

Once the amoeba optimizer is completed, a new DataContainer and AttributeMatrix are generated to store the dewarp parameters.  The actual dewarping of the montage is not performed in this filter.

## Parameters ##

| Name | Type | Description |
|------|------|------|
| Parameter Name | Parameter Type | Description of parameter... |
| **Montage Name** | GridMontage | Description of object... |
| **Attribute Matrix Name** | String | Description of object... |
| **IPF Colors Array Name** | String | Description of object... |
| **Max Iterations** | Int | Description of object... |
| **Function Convergence Tolerance** | Float | Description of object... |
| **Parameter Convergence Tolerance** | Float | Description of object... |
| **X Factors** | FloatVec7Type | Description of object... |
| **Y Factors** | FloatVec7Type | Description of object... |

## Required Geometry ##
Not Applicable

## Required Objects ##
Not Applicable

## Created Objects ##

| Kind | Default Name | Type | Component Dimensions | Description |
|------|--------------|-------------|---------|-----|
| **Data Container** | Dewarp Data | N/A | N/A | Description of object... |
| **Attribute Matrix** | Transform Matrix | Generic | N/A | Description of object... |
| **Element Attribute Array** | Transform | double | (14) | Description of object... |
| **Data Container Prefix** | Transformed_ | N/A | N/A | Description of object... |


## Example Pipelines ##

List the names of the example pipelines where this filter is used.

## License & Copyright ##

Please see the description file distributed with this plugin.

## DREAM3D Mailing Lists ##

If you need more help with a filter, please consider asking your question on the DREAM3D Users mailing list:
https://groups.google.com/forum/?hl=en#!forum/dream3d-users