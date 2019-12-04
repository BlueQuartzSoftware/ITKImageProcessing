# Blend #


## Group (Subgroup) ##

Processing (Processing)

## Description ##

This **Filter** takes a set of dewarp parameters from the CalcDewarpParameters filter and uses them to dewarp tiles within a GridMontage.  The parameters are expected to be in the format

> x' = a<sub>0</sub> * x + a<sub>1</sub> * y + a<sub>2</sub> * x<sup>2</sup> + a<sub>3</sub> * y<sup>2</sup> + a<sub>4</sub> * xy + a<sub>5</sub> * x<sup>2</sup>y + a<sub>6</sub> * xy<sup>2</sup>

> y' = b<sub>0</sub> * x + b<sub>1</sub> * y + b<sub>2</sub> * x<sup>2</sup> + b<sub>3</sub> * y<sup>2</sup> + b<sub>4</sub> * xy + b<sub>5</sub> * x<sup>2</sup>y + b<sub>6</sub> * xy<sup>2</sup>

where X and Y are offset by half of the target tile dimensions instead of starting at the tile's top-left corner.  The target tile dimensions are taken from the first or second row/column based on the number of rows and columns in the montage.  If there are more than two rows, the target height is taken from the second row.  Otherwise, the target height is taken from the first row.  If there are more than two columns, the target width is taken from the second column.  Otherwise, the target width is taken from the first column.

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
| **Data Container Prefix** | Transformed_ | N/A | N/A | Prefix to apply for dewarped DataContainers and GridMontage |
| **Attribute Array** | Mask | Element | (1) | Name of the created Mask array for each dewarped DataContainer |


## Example Pipelines ##

None

## License & Copyright ##

Please see the description file distributed with this plugin.

## DREAM3D Mailing Lists ##

If you need more help with a filter, please consider asking your question on the DREAM3D Users mailing list:
[DREAM.3D Users Google group!](https://groups.google.com/forum/?hl=en#!forum/dream3d-users)