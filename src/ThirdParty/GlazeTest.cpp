#ifndef __linux__

#include "test.h"
#include "Glaze_Common.h"


class GlazeTest: public TypeSafeTest<Glaze::VectorDouble,
                                     Glaze::VectorString,
                                     Glaze::GetValue>
{
    Glaze::GetValue<Perform>                                             testGetValueCatalog;
    Glaze::GetValue<Country>                                             testGetValueCountry;
    Glaze::GetValue<Twitter>                                             testGetValueTwitter;
    public:
        GlazeTest()
        {
            actionMap["performance/citm_catalog.json"]   = &testGetValueCatalog;
            actionMap["performance/canada.json"]         = &testGetValueCountry;
            actionMap["performance/twitter.json"]        = &testGetValueTwitter;
        }

        virtual const char* GetName()     const override    { return "Glaze"; }
        virtual const char* Type()        const override    { return "C++23";}
        virtual const char* GetFilename() const override    { return __FILE__; }
};

REGISTER_TEST(GlazeTest);

#endif
