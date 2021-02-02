.pragma library

var factorialCount = 0

function factorial(a)
{
    a = parseInt(a)

    if ( a > 0 ) {
        factorialCount += 1
        return a * factorial(a - 1)
    }

    return 1;
}

function factorialCallCount()
{
    return factorialCount
}
