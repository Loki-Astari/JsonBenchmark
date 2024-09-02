
# JsonBenchmark

## Benchmark Results:
Conformance [mac](https://LokiAstari.com/Json/Conformance.osx.html) [linux](https://LokiAstari.com/Json/Conformance.linux.html)  
Performance [mac](https://LokiAstari.com/Json/Performance.osx.html) [linux](https://LokiAstari.com/Json/Performance.linux.html)  

## Disclaimer
The original author (Milo Yip) of this benchmark is also the primary author of [RapidJSON](https://github.com/miloyip/rapidjson).  
The next author (Loki Astari) of this benchmarks is also the primary author of [ThorsSerializer](https://github.com/Loki-Astari/ThorsSerializer)  

## Introduction
These benchmarks were originally written by Milo in the project [nativejson-benchmarkback](https://github.com/miloyip/nativejson-benchmark) between 2014 and 2016. Milo no longer seems to be actively updating the project so I have am trying to resurrect the project. I have taken it upon my self to re-write the wrapper code in cleaner C++ (this is why it is not a fork of the original) and port over as many of the original libraries as I could (with as little modification to the original testing code as possible).

The original test harness interface written by Milo has been maintained in the file [src/ThirdParty/test.h](https://github.com/Loki-Astari/JsonBenchmark/blob/master/src/ThirdParty/test.h). Also the original tests in [data](https://github.com/Loki-Astari/JsonBenchmark/tree/master/data) remain the same, though the directory structure inside data may have changed and the files in the directory "data/validate_&ast;/&ast;.json" contain both input and output rather than raw json. Though not directly visible the php code that generates the results html and all the javascript have been taken directly from Milo's original project.

### Original Intent
You can look back at Milo's original project and read the [README](https://github.com/miloyip/nativejson-benchmark) over there. But basically he was trying to provide both conformance and performance metrics.

### New Intent
The intent of this library is to allow:

* New Json Libraries to be easily added for inclusion.
* New Test suites to be easily added for inclusion.
* New Test data for test suites.
* The results to be automatically published for both Mac and Linux.

We will allow readers of the tests to decide what the data means.

## Flexibility

All updates to this project will be accepted via pull requests. Please check that the projects continue to build correctly.

To make adding a new project simple there is a special branch for adding projects that does not include any other sub projects.

Step 1: Fork the repository.



```bash
> git checkout https://github.com/<You>/JsonBenchmark.git
> cd JsonBenchmark
> ./setUpNewProject https://github.com/<You>/<Project>.git
> git commit -a -m "Add base for project M"
> git push
> ./configure
> make
> ./runOneTest
```

This should:

* Create a new branch "<AddProject>"
* Add your project as a submodule inside the directory "thirdparty/<Project>".
* Remove all the other projects to make sure your build and development is simple.

You will need to:
* Modify the file "init/<Project>" to handle the building of your project (so that when I integrate it into the main project it will be built). If yours is a header only library then you can leave this file empty.
* Modify the file "src/ThirdParty/Makefile" Add any flags you need to build your project. Instructions in the file.
* Modify the file "src/ThirdParty/<Project>Test.cpp" to implement the functions you need. Instructions in the file.


When you are ready you can build with:

```bash
> make
> ./runOneTest
```


## Building the whole project:

It should be simple to checkout and build the project locally.

**BUT** things are never simple with C++ and multiple platforms.

I have tried to simplify things by adding scripts to set up all the dependant libraries. The following works on Mac and Linux.


```bash
> git checkout https://github.com/Loki-Astar/JsonBenchmark.git
> ./configure
> # To install standard packages that are used to run tests.
> ./initPlatform
> # To build all 3rd party JSON libraries.
> ./init3rdParty
> # Build the test platform.
> make
> ./runTests
````

### Tests

Executing Class | Data Directory | SetUp/TearDown Name | TestBase API Used | TestDescription
--------------- | -------------- | ------------------- | ----------------- | ---------------
FailChecker           | jsonchecker_fail  | &lt;Dir&gt;/&lt;File&gt;  | Parse()           | Tests that should all fail.
PassChecker           | jsonchecker_pass  | &lt;Dir&gt;/&lt;File&gt;  | Parse()           | Tests that should all pass.
ValidateFloat         | validate_float    | vector-double | ParseDouble()     | Float Value (as a string) and a floating point value. Make sure the string is correctly converted to a floating point value.
ValidateString        | validate_string   | vector-string | ParseString()     | String with escape sequences and utf-8 encoded string. Make sure the escaped string is correctly converted to utf-8.
RoundTripChecker      | roundtrip         | &lt;Dir&gt;/&lt;File&gt;  | Parse() Stringify()  | A Json object is read into internal representation then converted back to a string. Ignore space (not in a string) make sure they are the same.
PerformanceChecker    | performance       | &lt;Dir&gt;/&lt;File&gt;  |                      | Some big Json objects. See below:
PerformanceChecker    | performance       | &lt;Dir&gt;/&lt;File&gt;  | Parse()              | Parse: Parse Json string into object: Get execution time.
PerformanceChecker    | performance       | &lt;Dir&gt;/&lt;File&gt;  | Parse() Stringify()  | Stringify: Convert internal object to string (Object from Parse): Get execusion time.
PerformanceChecker    | performance       | &lt;Dir&gt;/&lt;File&gt;  | Parse() Prettify()   | Prettify: Like stringify but is supposed do generate neat Json: Get execution time.
PerformanceChecker    | performance       | &lt;Dir&gt;/&lt;File&gt;  | Parse() Statistics() | Statistics: Make sure the correct number of objects are created by Parse.
PerformanceChecker    | performance       | &lt;Dir&gt;/&lt;File&gt;  | SaxRoundtrip()       | SaxRoundtrip: Needs documentation.
PerformanceChecker    | performance       | &lt;Dir&gt;/&lt;File&gt;  | SaxStatistics()      | SaxStatistics: Needs documentation.

#### Adding a new Test Suite

Adding a new test suite is a two part processes.

* Add a new directory under data to hold the test data.
* Add a test suite

#### Adding new Test data

Adding new test data to an existing test suite is as simple as adding a file to the appropriate directory under data.  
The test harness will automatically loop through all test data files in the directory belonging to the test suite.  
NOTE: All tests files must end in ".json" if the file name includes the name "EXCLUDE" it will be ignored.


