[![Build Status](https://travis-ci.org/Loki-Astari/JsonBenchmark.svg?branch=master)](https://travis-ci.org/Loki-Astari/JsonBenchmark)

# JsonBenchmark

## Benchmark Results:
Conformance [mac](LokiAstari.com/Json/Conformance.osx.html) [linux](LokiAstari.com/Json/Conformance.linux.html)  
Performance [max](LokiAstari.com/Json/Performance.osx.html) [linux](LokiAstari.com/Json/Performance.linux.html)  

## Disclaimer
The original author (Milo Yip) of this benchmark is also the primary author of [RapidJSON](https://github.com/miloyip/rapidjson).  
The next author (Loki Astari) of this benchmarks is also the primary author of [ThorsSerializer](https://github.com/Loki-Astari/ThorsSerializer)  

## Introduction
These benchmarks were originally written by Milo in the project [nativejson-benchmarkback](https://github.com/miloyip/nativejson-benchmarkback) between 2014 and 2016. Milo no longer seems to be actively updating the project so I have am trying to resurrect the project. I have taken it upon my self to re-write the wrapper code in cleaner C++ (this is why it is not a fork of the original) and port over as many of the original libraries as I could (with as little modification to the original testing code as possible).

The original test harness interface written by Milo has been maintained in the file [src/ThirdParty/test.h](https://github.com/Loki-Astari/JsonBenchmark/blob/master/src/ThirdParty/test.h). Also the original tests in [data](https://github.com/Loki-Astari/JsonBenchmark/tree/master/data) remain the same, though the directory structure inside data may have changed and the files in the directory "data/validate&underscore;&astrix;/&astrix;.json" contain both input and output rather than raw json. Though not directly visible the php code that generates the results html and all the javascript have been taken directly from Milo's original project.

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

All updates to this project will be accepted via pull requests. Please check that the projects continue to build correctly. The easy way to do this is via a [travis-ci](https://travis-ci.org) account. This is free you simply need to link your github account and allow travis-ci accesses to the JsonBenchmark repository.

Create a fork of this project. Then checkout your fork and build it locally.

````bash
> git checkout git@github.com:<You>/JsonBenchmark.git
> cd JsonBenchmark
> git submodule update --init --recursive
> ./configure
> make
````

### Adding a new Json Library

Run the script `scripts/AddJsonLibrary`

Running this script will generate the file `buildJsonLibrary`. Running this script will add the appropriate files to automatically generate the appropriate libraies.

After running these two scripts you will simply need to fill out the C++ to use the the Json library.

### Tests

Test Suite | Executing Class | Data Directory | SetUp/TearDown Name | TestBase API Used | TestDescription
---------- | --------------- | -------------- | ------------------- | ----------------- | ---------------
Fail        | FailChecker           | jsonchecker_fail  | <Dir>/<File>  | Parse()           | Tests that should all fail.
Pass        | PassChecker           | jsonchecker_pass  | <Dir>/<File>  | Parse()           | Tests that should all pass.
Float       | ValidateFloat         | validate_float    | vector-double | ParseDouble()     | Float Value (as a string) and a floating point value. Make sure the string is correctly converted to a floating point value.
String      | ValidateString        | validate_string   | vector-string | ParseString()     | String with escape sequences and utf-8 encoded string. Make sure the escaped string is correctly converted to utf-8.
Round Trip  | RoundTripChecker      | roundtrip         | <Dir>/<File>  | Parse() Stringify()  | A Json object is read into internal representation then converted back to a string. Ignore space (not in a string) make sure they are the same.
Performance | PerformanceChecker    | performance       | <Dir>/<File>  |                      | Some big Json objects. See below:
Performance | PerformanceChecker    | performance       | <Dir>/<File>  | Parse()              | Parse: Parse Json string into object: Get execution time.
Performance | PerformanceChecker    | performance       | <Dir>/<File>  | Parse() Stringify()  | Stringify: Convert internal object to string (Object from Parse): Get execusion time.
Performance | PerformanceChecker    | performance       | <Dir>/<File>  | Parse() Prettify()   | Prettify: Like stringify but is supposed do generate neat Json: Get execution time.
Performance | PerformanceChecker    | performance       | <Dir>/<File>  | Parse() Statistics() | Statistics: Make sure the correct number of objects are created by Parse.
Performance | PerformanceChecker    | performance       | <Dir>/<File>  | SaxRoundtrip()       | SaxRoundtrip: Needs documentation.
Performance | PerformanceChecker    | performance       | <Dir>/<File>  | SaxStatistics()      | SaxStatistics: Needs documentation.

#### Adding a new Test Suite

Adding a new test suite is a two part processes.

* Add a new directory under data to hold the test data.
* Add a test suite

#### Adding new Test data

Adding new test data to an existing test suite is as simple as adding a file to the appropriate directory under data.  
The test harness will automatically loop through all test data files in the directory belonging to the test suite.  
NOTE: All tests files must end in ".json" if the file name includes the name "EXCLUDE" it will be ignored.


