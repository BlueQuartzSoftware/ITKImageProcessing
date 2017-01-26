// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
//Auto includes
${TestsIncludeName}

// Copied from sitkKernel.h (SimpleITK)
namespace itk
{
namespace simple
{

enum KernelEnum {
  sitkAnnulus,
  sitkBall,
  sitkBox,
  sitkCross,
  sitkPolygon3,
  sitkPolygon4,
  sitkPolygon5,
  sitkPolygon6,
  sitkPolygon7,
  sitkPolygon8,
  sitkPolygon9
};

} // end namespace simple
} // end namespace itk

// end of Copied from sitkKernel.h (SimpleITK)

class ${FilterName}Test: public ITKTestBase
{

  public:
    ${FilterName}Test() {}
    virtual ~${FilterName}Test() {}

${FilterTests}


  // -----------------------------------------------------------------------------
  //
  // -----------------------------------------------------------------------------
  void operator()() override
  {
    int err = EXIT_SUCCESS;

    DREAM3D_REGISTER_TEST( this->TestFilterAvailability("${FilterName}") );

${RegisterTests}
    if(SIMPL::unittest::numTests == SIMPL::unittest::numTestsPass)
    {
      DREAM3D_REGISTER_TEST( this->RemoveTestFiles() )
    }
  }

  private:
    ${FilterName}Test(const ${FilterName}Test&); // Copy Constructor Not Implemented
    void operator=(const ${FilterName}Test&); // Operator '=' Not Implemented
};

