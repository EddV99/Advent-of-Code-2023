#include <climits>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <sys/types.h>
#include <tuple>
#include <vector>

/**
 * Declarations
 */
const long NOT_VALID = -1;
struct range;
struct mapping;
struct almanac;

void part1(std::string);
void parseFile(std::shared_ptr<std::ifstream> file, almanac&, bool);
void parseMapping(std::vector<mapping>&, std::vector<std::string>);

void part2(std::string);

/**
 * Util Helpers
 */
std::shared_ptr<std::ifstream> openFile(std::string filePath) {
    std::shared_ptr<std::ifstream> file = std::make_shared<std::ifstream>(
            filePath);
    if (!file->is_open()) {
        std::cout << "Coudn't find input file\n";
        exit(1);
    }
    return file;
}
std::vector<std::string> tokenize(std::string line) {
    std::vector<std::string> tokens;

    std::stringstream stream(line);
    std::string save;

    while (std::getline(stream, save, ' ')) { tokens.push_back(save); }
    return tokens;
}

/**
 * Main Method
 */
int main(int argc, char** argv) {
    std::string inputFile;
    if (argc > 1) inputFile = std::string(argv[1]);
    else {
        std::cout << "No input file given\n";
        return 1;
    }
    part1(inputFile);
    part2(inputFile);

    return 0;
};

/**
 * structs 
 */

/**
 * Represents a range [start, end]
 */
struct range {
    long start;
    long end;
    range(long s, long e) : start(s), end(e) {}
    range() : start(NOT_VALID), end(NOT_VALID) {}

    bool isValid() { return start != NOT_VALID && end != NOT_VALID; }

    bool inRange(long target) { return target >= start && target <= end; }

    bool hasOverlap(range other) {
        return (start <= other.end) && (end >= other.start);
    }
    /*
     * Offset from start to target
     *  Assume we know target is in range
     */
    long offset(long target) { return target - start; }
};

/**
 * Represents a mapping
 */
struct mapping {
    range from;
    range to;
    mapping(range f, range t) : from(f), to(t) {}

    bool has(long target) { return from.inRange(target); }
    /*
     * Assumes target already checked if it's in the mapping
     */
    long find(long target) { return to.start + from.offset(target); }


    /**
     * Get Overlap
     * Assumes their is overlap
     * Will return extra if not mapped
     */
    std::tuple<mapping, range, range> overlap(range target) {
        if (target.start < from.start) {
            long offset = std::abs(target.end - from.start);

            long fromEnd = target.end;

            mapping result({from.start, fromEnd},
                           {to.start, to.start + offset});

            range extra(target.start, from.start - 1);
            range extra2(-1, -1);  // no extra

            if (target.end > from.end) {
                extra2.start = from.end + 1;
                extra2.end = target.end;

                fromEnd = from.end;
                offset = 0;

                result.from = {from.start, fromEnd};
                result.to = {to.start, to.end + offset};
            }

            return std::make_tuple(result, extra, extra2);

        } else if (target.end > from.end) {
            long offset = std::abs(from.end - target.start);
            mapping result({target.start, from.end}, {to.end - offset, to.end});

            range extra(from.end + 1, target.end);
            range extra2(-1, -1);  // no extra

            return std::make_tuple(result, extra, extra2);

        } else if (target.start == from.start && target.end == from.end) {
            mapping result({from.start, from.end}, {to.start, to.end});
            range extra(-1, -1);   // no extra
            range extra2(-1, -1);  // no extra


            return std::make_tuple(result, extra, extra2);
        } else {
            long offsetStart = std::abs(target.start - from.start);
            long offsetEnd = std::abs(target.start - from.start)
                           + std::abs(target.end - target.start);

            mapping result({target.start, target.end},
                           {to.start + offsetStart, to.start + offsetEnd});

            range extra(-1, -1);   // no extra
            range extra2(-1, -1);  // no extra

            return std::make_tuple(result, extra, extra2);
        }
    }
};

/**
 * Represents the whole almanac
 */
struct almanac {
    std::vector<long> seedsPart1;
    std::vector<range> seedsPart2;
    std::vector<mapping> seedToSoil;
    std::vector<mapping> soilToFertilizer;
    std::vector<mapping> fertilizerToWater;
    std::vector<mapping> waterToLight;
    std::vector<mapping> lightToTemperature;
    std::vector<mapping> temperatureToHumidity;
    std::vector<mapping> humidityToLocation;

    /**
     * Part 1 helper method
     * Just checks if target is part of list of mapping. Return itself if not
     * found in any map.
     */
    long findInMap(long target, std::vector<mapping> maps) {
        for (int i = 0; i < maps.size(); i++) {
            mapping map = maps[i];
            if (map.has(target)) { return map.find(target); }
        }

        return target;
    }

    long findLowestLocationPart1() {
        long lowest = LONG_MAX;

        std::vector<std::vector<mapping>> allMaps;
        allMaps.push_back(seedToSoil);
        allMaps.push_back(soilToFertilizer);
        allMaps.push_back(fertilizerToWater);
        allMaps.push_back(waterToLight);
        allMaps.push_back(lightToTemperature);
        allMaps.push_back(temperatureToHumidity);
        allMaps.push_back(humidityToLocation);

        for (auto& seed : seedsPart1) {
            long target = seed;
            for (auto& map : allMaps) { target = findInMap(target, map); }

            if (target < lowest) lowest = target;
        }

        return lowest;
    }
    std::vector<range> getDstFromRanges(std::vector<range> targets,
                                        std::vector<mapping> maps) {
        std::vector<range> dst;

        while (!targets.empty()) {
            // get any target to check
            range target = targets.back();
            targets.pop_back();  // dont forget to delete it

            bool found = false;
            for (auto& map : maps) {
                if (target.hasOverlap(map.from)) {
                    found = true;

                    auto overlap = map.overlap(target);

                    dst.push_back(std::get<0>(overlap).to);

                    if (std::get<1>(overlap).isValid()) {
                        targets.emplace_back(std::get<1>(overlap));
                        if (std::get<2>(overlap).isValid())
                            targets.emplace_back(std::get<2>(overlap));
                    }
                    break;
                }
            }

            // looked through every map and nothing so it falls through
            if (!found) { dst.push_back(target); }
        }

        return dst;
    }

    long findLowestLocationPart2() {
        std::vector<std::vector<mapping>> allMaps;
        allMaps.push_back(seedToSoil);
        allMaps.push_back(soilToFertilizer);
        allMaps.push_back(fertilizerToWater);
        allMaps.push_back(waterToLight);
        allMaps.push_back(lightToTemperature);
        allMaps.push_back(temperatureToHumidity);
        allMaps.push_back(humidityToLocation);

        // initial target range
        std::vector<range> destinations = seedsPart2;

        for (int i = 0; i < allMaps.size(); i++)
            destinations = getDstFromRanges(destinations, allMaps[i]);

        /**
         * last destinations should be the locations, so
         * look through them and get the smallest one
         */
        long lowest = LONG_MAX;
        for (int i = 0; i < destinations.size(); i++) {
            if (destinations[i].start < lowest) lowest = destinations[i].start;
        }
        return lowest;
    }
};

/**
 * parsing
 */

/**
 * enums to help with parsing
 */
enum parseState {
    SEEDS,
    SEED_TO_SOIL,
    SOIL_TO_FERTILIZER,
    FERTILIZER_TO_WATER,
    WATER_TO_LIGHT,
    LIGHT_TO_TEMPERATURE,
    TEMPERATURE_TO_HUMIDITY,
    HUMIDITY_TO_LOCATION
};
void parseMapping(std::vector<mapping>& map, std::vector<std::string> tokens) {
    long from = std::stoul(tokens[1]);
    long to = std::stoul(tokens[0]);
    long length = std::stoul(tokens[2]);

    range fromRange(from, from + length - 1);
    range toRange(to, to + length - 1);

    mapping mapped(fromRange, toRange);

    map.push_back(mapped);
}
void parseFile(std::shared_ptr<std::ifstream> file, almanac& puzzle,
               bool isPart1) {
    std::string line;
    parseState state = SEEDS;
    while (std::getline(*file, line)) {
        if (line.empty()) continue;
        if (line.find("seed-to-soil") != std::string::npos) {
            state = SEED_TO_SOIL;
            continue;
        } else if (line.find("soil-to-fertilizer") != std::string::npos) {
            state = SOIL_TO_FERTILIZER;
            continue;
        } else if (line.find("fertilizer-to-water") != std::string::npos) {
            state = FERTILIZER_TO_WATER;
            continue;
        } else if (line.find("water-to-light") != std::string::npos) {
            state = WATER_TO_LIGHT;
            continue;
        } else if (line.find("light-to-temperature") != std::string::npos) {
            state = LIGHT_TO_TEMPERATURE;
            continue;
        } else if (line.find("temperature-to-humidity") != std::string::npos) {
            state = TEMPERATURE_TO_HUMIDITY;
            continue;
        } else if (line.find("humidity-to-location") != std::string::npos) {
            state = HUMIDITY_TO_LOCATION;
            continue;
        }

        std::vector tokens = tokenize(line);

        switch (state) {
            case SEEDS: {
                if (isPart1) {
                    for (int i = 1; i < tokens.size(); i++)
                        puzzle.seedsPart1.push_back(std::stoul(tokens[i]));
                } else {
                    for (int i = 1; i < tokens.size(); i += 2) {
                        long from = std::stoul(tokens[i]);
                        long length = std::stoul(tokens[i + 1]);
                        range seedRange(from, from + length - 1);
                        puzzle.seedsPart2.push_back(seedRange);
                    }
                }
                break;
            }
            case SEED_TO_SOIL: {
                parseMapping(puzzle.seedToSoil, tokens);
                break;
            }
            case SOIL_TO_FERTILIZER: {
                parseMapping(puzzle.soilToFertilizer, tokens);
                break;
            }
            case FERTILIZER_TO_WATER: {
                parseMapping(puzzle.fertilizerToWater, tokens);
                break;
            }
            case WATER_TO_LIGHT: {
                parseMapping(puzzle.waterToLight, tokens);
                break;
            }
            case LIGHT_TO_TEMPERATURE: {
                parseMapping(puzzle.lightToTemperature, tokens);
                break;
            }
            case TEMPERATURE_TO_HUMIDITY: {
                parseMapping(puzzle.temperatureToHumidity, tokens);
                break;
            }
            case HUMIDITY_TO_LOCATION: {
                parseMapping(puzzle.humidityToLocation, tokens);
                break;
            }
        }
    }
}

/**
 * Part 1
 */
void part1(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);
    almanac puzzle;
    parseFile(file, puzzle, true);
    long lowest = puzzle.findLowestLocationPart1();
    std::cout << "Part 1 Lowest Location is " << lowest << "\n";
}

/**
 * Part 2
 */

void part2(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(inputFile);
    almanac puzzle;
    parseFile(file, puzzle, false);

    long lowest = puzzle.findLowestLocationPart2();
    std::cout << "Part 2 Lowest Location is " << lowest << "\n";
}
