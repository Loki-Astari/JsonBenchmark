#include "SimdjsonDomTypes.hpp"

// OK: The TypeSafeTest test is a template type that has a handler for each test case.
//     There are three default handlers that you need to provide.
//          class VectorDouble: public TestAction
//          class VectorString: public TestAction
//          template<typename T>
//          class GetValue: public TestAction
//
// See the file: SimdjsonDomTypes.h for the framework.
//
// This should cover most cases.
// If you need to do some special handling of types you can override the default actions.
// See comments blow:

class SimdjsonDomTest : public TypeSafeTest<SimdjsonDomTypes::VectorDouble,      // Type for parsing array of single double.
	SimdjsonDomTypes::VectorString,      // Type ofr parsing array of single string
	SimdjsonDomTypes::GetValue>          // Template template class. This is templatezed with the type
	// That needs to be read from the json string.
{
	SimdjsonDomTypes::GetValue<canada_message>                    testGetValueCountry;
	SimdjsonDomTypes::GetValue<twitter_message>                   testGetValueTwitter;
	SimdjsonDomTypes::GetValue<citm_catalog_message>              testGetValueCatalog;
public:
	SimdjsonDomTest()
	{
		actionMap["performance/canada.json"] = &testGetValueCountry;
		actionMap["performance/twitter.json"] = &testGetValueTwitter;
		actionMap["performance/citm_catalog.json"] = &testGetValueCatalog;
	}

	virtual const char* GetName()     const override { return "SimdjsonDom"; }
	virtual const char* Type()        const override { return "C++"; }
	virtual const char* GetFilename() const override { return __FILE__; }
};

REGISTER_TEST(SimdjsonDomTest);