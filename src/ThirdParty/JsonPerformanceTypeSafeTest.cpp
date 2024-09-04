
#include "test.h"
#include "TypeSafe.h"
#include "JsonPerformanceTypes.h"


// OK: The TypeSafeTest test is a template type that has a handler for each test case.
//     There are three default handlers that you need to provide.
//          class VectorDouble: public TestAction
//          class VectorString: public TestAction
//          template<typename T>
//          class GetValue: public TestAction
//
// See the file: JsonPerformanceTypes.h for the framework.
//
// This should cover most cases.
// If you need to do some special handling of types you can override the default actions.
// See comments blow:

class JsonPerformanceTest: public TypeSafeTest<JsonPerformanceTypes::VectorDouble,      // Type for parsing array of single double.
                                               JsonPerformanceTypes::VectorString,      // Type ofr parsing array of single string
                                               JsonPerformanceTypes::GetValue>          // Template template class. This is templatezed with the type
                                                                                        // That needs to be read from the json string.
{
    // Note This type must enherit from TestAction (See TypeSafe.h)
    MyTypeForHandlingRoundTrip24        roundTrip24;
    public:
        JsonPerformanceTest()
        {
            // Each member of "actionMap" points at an object that knows the type
            // information for decoding the json. You can override any test by making them
            // point at variables in this class.
            //
            // The TypeSafeTest base class sets up default objects based on the
            // template parameter types.
            actionMap["roundtrip/roundtrip24.json"] = &roundTrip24;
        }

        virtual const char* GetName()     const override    { return "JsonPerformance"; }
        virtual const char* Type()        const override    { return "C++";}
        virtual const char* GetFilename() const override    { return __FILE__; }
};

REGISTER_TEST(JsonPerformanceTest);

