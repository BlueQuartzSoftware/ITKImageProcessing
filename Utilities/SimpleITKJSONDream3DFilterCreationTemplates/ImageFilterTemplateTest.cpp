// File automatically generated

// -----------------------------------------------------------------------------
// Insert your license & copyright information here
// -----------------------------------------------------------------------------

#include "ITKTestBase.h"
//Auto includes
${TestsIncludeName}

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

