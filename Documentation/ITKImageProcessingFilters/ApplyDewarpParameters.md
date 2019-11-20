# Blend #


## Group (Subgroup) ##

Processing (Processing)

## Description ##

This **Filter** takes a set of dewarp parameters from the CalcDewarpParameters filter and uses them to dewarp tiles within a GridMontage.  The parameters are expected to be in the format

X' = a_0 * X + a_1 * Y + a_2 * X^2 + a_3 * Y^2 + a_4 * XY + a_5 * X^2Y + a_6 * XY^2
Y' = a_7 * X + a_8 * Y + a_9 * X^2 + a_10 * Y^2 + a_11 * XY + a_12 * X^2Y + a_13 * XY^2

where X and Y are offset based on the center of the average tile instead of the top-left corner.

## Parameters ##

| Name | Type | Description |
|------|------|------|
| **GridMontage** | Montage | Montage to dewarp |
| **Attribute Matrix Name** | String | Name of the AttributeMatrix that should be dewarped for each tile |
| **Transform Array** | DataArrayPath | DataArrayPath to the transformation parameters |

## Required Geometry ##
Not Applicable

## Required Objects ##
Not Applicable

## Created Objects ##

| Kind | Default Name | Type | Component Dimensions | Description |
|------|--------------|-------------|---------|-----|
| **Data Container Prefix** | Transformed Data Container Prefix | N/A | N/A | Prefix to apply for dewarped DataContainers |
| **Attribute Array** | Mask Array Name | Element | (1) | Name of the created Mask array for each dewarped DataContainer |


## Example Pipelines ##

List the names of the example pipelines where this filter is used.

## License & Copyright ##

Please see the description file distributed with this plugin.

## DREAM3D Mailing Lists ##

If you need more help with a filter, please consider asking your question on the DREAM3D Users mailing list:
https://groups.google.com/forum/?hl=en#!forum/dream3d-users