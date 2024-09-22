#include "SimdjsonOnDemandTypes.hpp"

// OK: The TypeSafeTest test is a template type that has a handler for each test case.
//     There are three default handlers that you need to provide.
//          class VectorDouble: public TestAction
//          class VectorString: public TestAction
//          template<typename T>
//          class GetValue: public TestAction
//
// See the file: SimdjsonOnDemandTypes.h for the framework.
//
// This should cover most cases.
// If you need to do some special handling of types you can override the default actions.
// See comments blow:
class SimdjsonOnDemandTest : public TypeSafeTest<SimdjsonOnDemandTypes::VectorDouble,      // Type for parsing array of single double.
	SimdjsonOnDemandTypes::VectorString,      // Type ofr parsing array of single string
	SimdjsonOnDemandTypes::GetValue>          // Template template class. This is templatezed with the type
	// That needs to be read from the json string.
{
	SimdjsonOnDemandTypes::GetValue<canada_message>                    testGetValueCountry;
	SimdjsonOnDemandTypes::GetValue<twitter_message>                   testGetValueTwitter;
	SimdjsonOnDemandTypes::GetValue<citm_catalog_message>              testGetValueCatalog;
public:
	SimdjsonOnDemandTest()
	{
		actionMap["performance/canada.json"] = &testGetValueCountry;
		actionMap["performance/twitter.json"] = &testGetValueTwitter;
		actionMap["performance/citm_catalog.json"] = &testGetValueCatalog;
	}

	virtual const char* GetName()     const override { return "SimdjsonOnDemand"; }
	virtual const char* Type()        const override { return "C++"; }
	virtual const char* GetFilename() const override { return __FILE__; }
};

REGISTER_TEST(SimdjsonOnDemandTest);