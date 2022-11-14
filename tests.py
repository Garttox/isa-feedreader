#compares stdout and stderr with expected outputs from test files
#test files must be named same as test. File with expected stderr have name format "<testName>Err"
#results will be printed to stdout
from subprocess import Popen, PIPE

folder = "tests" #folder with test files
executable = "./feedreader" #executable of tested program
results = [] #tests results

# [testName, aruments, compare stdout]
tests = [
    #argParser
    ["invalidOption1", ["-x"], False],
    ["invalidOption2", ["-a","-T","-u"], False],
    ["invalidOption3", ["-f"], False],
    ["invalidOption4", ["https://what-if.xkcd.com/feed.atom", "-f"], False],
    ["invalidOption5", ["https://what-if.xkcd.com/feed.atom", "-f", "tests/res/feedfile1"], False],
    #url/feedfileReader
    ["invalidUrl1", ["httpps://what-if.xkcd.com/feed.atom"], False],
    ["invalidUrl2", ["https://what-if.xkcd..com/feed.atom"], False],
    ["ivalidFeedfileUrl1", ["-f","tests/res/feedfile2"], False],
    ["missingFeedfile", ["-f","wrong/path"], False],
    #feedReader
    ["ivalidFeedUrl1", ["https://what-if.xkcddd.com/feed.atom"], False],
    ["ivalidFeedUrl2", ["https://what-if.xkcd.com/something/feed.atom"], False],
    #feedReader cert
    ["ivalidFeedCert1", ["https://what-if.xkcd.com/feed.atom","-u","-T","-C","/dev/null"], False],
    #xmlReader
    ["invalidXML", ["https://www.vut.cz/"], False],
    #valid inputs tests (if feed changes, tests will fail and should be generated again)
    ["validTest1", ["https://what-if.xkcd.com/feed.atom","-T"], True],
    ["validTest2", ["https://www.theregister.com/headlines.rss","-T","-a"], True],
    ["validTest3", ["https://www.fit.vut.cz/fit/news-rss/","-T","-a","-u"], True]
]

#["validUrl1", ["https://what-if.xkcd.com:465/feed.atom"], False],
#["validUrl2", ["http://what-if.xkcd.com/feed.atom"], False],

def readFile(fileName):
    with open(folder+"/"+fileName) as f:
        fileContent = f.read()
    return fileContent

#execute test
def executeTest(test):
    process = Popen([executable] + test[1], shell=False, stdout=PIPE, stderr=PIPE)
    stdout, stderr = process.communicate()
    if(test[2]):
        stdoutRes = (stdout == readFile(test[0]).encode()) #stdout compare file
    else:
        stdoutRes = "none*"
    stderrRes = (stderr == readFile(test[0]+"Err").encode()) #stderr compare file
    results.append((test[0], stdoutRes, stderrRes))

#evaluate tests from variable tests
def evaluateTests():
    for test in tests:
        executeTest(test)
    passedCount = 0
    print("Test name\tStdout check\tStderr check\tresult")
    print("----------------------------------------------------")
    for result in results:
        passed = "not passed"
        if(result[1] and result[2]):
            passed = "passed"
            passedCount += 1
        print(result[0] + ":\tstdout: " + str(result[1]) + "\tstderr: " + str(result[2]) + "\t" + passed)
    print("+-------------------------------+")
    passedPercent = str(100*(passedCount/len(results)))
    print("| Passed: " + str(passedCount) + "/"+ str(len(results)) +" | " + passedPercent + "%\t|")
    print("+-------------------------------+")
    print("*stdout is not checked in this test")


evaluateTests()