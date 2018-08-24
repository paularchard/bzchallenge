# bzchallenge

## Getting Started

This repository contains sample code for a C++ challenge assignment. I won't describe the assignment here in case anyone finds it when they shoudn't.

### Prerequisites

You will need a C++ compiler (e.g. GNU GCC) and CMake to build and run this project. You will also need a recent version of Boost installed.

### Installing

To build the project, first clone the repository:
```
git clone https://github.com/paularchard/bzchallenge.git
```

Then make a build directory and change to it:

```
mkdir build; cd build
```
Then build the Makefile using CMake:
```
cmake ..
```
Then type "make" to build the project.
```
make
```

## Running the tests

To run the tests from the build directory, type:
```
../run_all_tests
```

## Running the program

```
Usage: ./bzchallenge filename
Usage: ./bzchallenge start-date end-date

Options:
  filename      JSON file containing price data
  start-date    First date for price info (yyyy-mm-dd)
  end-date      Last date for price info (yyyy-mm-dd)

Summary:
Retrieves price data from a file (first form) or from an online service (second form)
and provides statistics on the data. The second format requires a start and end date
which specify the date range to process data for.
```

To run the program, you have two options. Either provide a text file containing the sample data (the sample "prices.json" is provided), e.g.
```
./bzchallenge ../prices.json
```
or provide a start date and end date and the data will be fetched for you, e.g.
```
./bzchallenge 2018-01-01 2018-01-20
```

## License

This project is licensed under the GNU Affero General Public License - see the [LICENSE.md](LICENSE.md) file for details

