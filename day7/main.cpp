#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

/**
 * declarations
 */
std::vector<std::string> tokenize(const std::string& line);
std::shared_ptr<std::ifstream> openFile(std::string filePath);

void part1(std::string inputFile);

void part2(std::string inputFile);

/**
 * util
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

std::vector<std::string> tokenize(const std::string& line) {
    std::vector<std::string> tokens;

    std::stringstream stream(line);
    std::string save;

    while (std::getline(stream, save, ' ')) {
        if (!save.empty()) tokens.push_back(save);
    }
    return tokens;
}

/**
 * main function
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
 * Camel Cards Game
 */

enum CamelSuits : int {
    TWO = 2,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JOKER,
    QUEEN,
    KING,
    ACE
};

bool isPart1 = true;

struct camelGame;

struct Hand {
    int hand[5];
    int bid;

    ~Hand() = default;
};

/**
 * Five of a kind
 * Four of a kind
 * Full house
 * Three of a kind
 * Two pair
 * One pair
 * High card
 */
std::vector<int> countSuits(const Hand hand) {
    std::vector<int> count(20, 0);

    count[hand.hand[0]] += 1;
    count[hand.hand[1]] += 1;
    count[hand.hand[2]] += 1;
    count[hand.hand[3]] += 1;
    count[hand.hand[4]] += 1;

    return count;
}

bool isFiveOfKind(const Hand hand) {
    std::vector<int> count = countSuits(hand);
    int pairCount = 0;
    bool hasThree = false;
    bool hasFour = false;
    bool hasFive = false;

    for (int i = CamelSuits::TWO; i <= CamelSuits::ACE; i++) {
        if (count[i] == 2) pairCount++;
        if (count[i] == 3) hasThree = true;
        if (count[i] == 4) hasFour = true;
        if (count[i] == 5) hasFive = true;
    }
    if (!isPart1) {
        int jokerCount = count[JOKER];
        if (jokerCount == 1) {
            if (hasFour) return true;
            return false;
        } else if (jokerCount == 2) {
            if (hasThree) return true;
            return false;
        } else if (jokerCount == 3) {
            if (pairCount == 1) return true;
            return false;
        } else if (jokerCount >= 4)
            return true;
    }
    return hasFive;
}
bool isFourOfKind(const Hand hand) {
    std::vector<int> count = countSuits(hand);

    int pairCount = 0;
    bool hasThree = false;
    bool hasFour = false;

    for (int i = CamelSuits::TWO; i <= CamelSuits::ACE; i++) {
        if (count[i] == 2) pairCount++;
        if (count[i] == 3) hasThree = true;
        if (count[i] == 4) hasFour = true;
    }

    if (!isPart1) {
        int jokerCount = count[JOKER];

        // J4A43
        if (jokerCount == 1) {
            if (hasThree) return true;
            return false;
        } else if (jokerCount == 2) {
            // JJ335
            if (pairCount == 2) return true;
            return false;
        } else if (jokerCount == 3) {
            // JJJ23
            if (pairCount == 0) return true;
            else
                return false;
        }

        if (jokerCount >= 4) return false;
    }

    return hasFour;
}

bool isFullHouse(const Hand hand) {
    std::vector<int> count = countSuits(hand);
    int pairCount = 0;
    bool hasThree = false;
    bool hasFour = false;
    for (int i = CamelSuits::TWO; i <= CamelSuits::ACE; i++) {
        if (count[i] == 2) pairCount++;
        if (count[i] == 3) hasThree = true;
        if (count[i] == 4) hasFour = true;
    }

    if (!isPart1) {
        int jokerCount = count[JOKER];

        // J2734
        if (jokerCount == 1) {
            if (pairCount == 2) return true;
            return false;
        }
        if (jokerCount >= 2) return false;
    }
    return (pairCount == 1) && hasThree;
}

bool isThreeOfKind(const Hand hand) {
    std::vector<int> count = countSuits(hand);
    bool hasThree = false;
    int pairCount = 0;

    for (int i = CamelSuits::TWO; i <= CamelSuits::ACE; i++) {
        if (count[i] == 2) pairCount++;
        if (count[i] == 3) hasThree = true;
    }


    if (!isPart1) {
        // JJ835
        int jokerCount = count[JOKER];

        if (jokerCount == 1) {
            if ((pairCount == 1) && !hasThree) return true;
            return false;
        } else if (jokerCount == 2) {
            if ((pairCount == 1) && !hasThree) return true;
            else
                return false;
        } else if (jokerCount > 0)
            return false;
    }

    return hasThree && !(pairCount > 0);
}
bool isTwoPair(const Hand hand) {
    std::vector<int> count = countSuits(hand);
    int pairCount = 0;
    bool hasThree = false;

    for (int i = CamelSuits::TWO; i <= CamelSuits::ACE; i++) {
        if (count[i] == 2) pairCount++;
        if (count[i] == 3) hasThree = true;
    }

    if (!isPart1) {
        int jokerCount = count[JOKER];

        if (jokerCount >= 1) return false;
    }


    return pairCount == 2;
}
bool isOnePair(const Hand hand) {
    std::vector<int> count = countSuits(hand);
    int pairCount = 0;
    bool hasThree = false;
    // for part 2
    bool hasFour = false;
    bool hasFive = false;


    for (int i = CamelSuits::TWO; i <= CamelSuits::ACE; i++) {
        if (count[i] == 2) pairCount++;
        if (count[i] == 3) hasThree = true;
        if (count[i] == 4) hasFour = true;
        if (count[i] == 5) hasFive = true;
    }

    if (!isPart1) {
        int jokerCount = count[JOKER];

        if (jokerCount == 1) {
            if (hasFour || hasThree || hasFive || (pairCount >= 1))
                return false;
            return true;
        } else if (jokerCount > 0)
            return false;
    }

    return (pairCount == 1) && !hasThree;
}
bool isHighCard(const Hand hand) {
    std::vector<int> count = countSuits(hand);

    if (!isPart1) {
        // Just having joker guarantees a pair
        if (count[JOKER] >= 1) return false;
    }

    for (int i = CamelSuits::TWO; i <= CamelSuits::ACE; i++) {
        if (count[i] > 1) return false;
    }

    return true;
}

int getHandStrength(const Hand hand) {
    if (isFiveOfKind(hand)) return 7;
    if (isFourOfKind(hand)) return 6;
    if (isFullHouse(hand)) return 5;
    if (isThreeOfKind(hand)) return 4;
    if (isTwoPair(hand)) return 3;
    if (isOnePair(hand)) return 2;

    return 1;
}

bool isEqual(const Hand a, const Hand b) {
    return getHandStrength(a) == getHandStrength(b);
}

bool isStronger(const Hand a, const Hand b) {
    if (isEqual(a, b)) {
        for (int i = 0; i < 5; i++) {
            int suitA = a.hand[i];
            int suitB = b.hand[i];

            if (!isPart1) {
                if (suitA == JOKER) suitA = 0;
                if (suitB == JOKER) suitB = 0;
            }

            if (suitA > suitB) return true;
            else if (suitA < suitB)
                return false;
        }
        return false;
    }

    return getHandStrength(a) > getHandStrength(b);
}

std::vector<CamelSuits> stringToHand(const std::string& str) {
    std::vector<CamelSuits> suits;
    for (const auto& ch : str) {
        if (ch == '2') { suits.push_back(CamelSuits::TWO); }
        if (ch == '3') { suits.push_back(CamelSuits::THREE); }
        if (ch == '4') { suits.push_back(CamelSuits::FOUR); }
        if (ch == '5') { suits.push_back(CamelSuits::FIVE); }
        if (ch == '6') { suits.push_back(CamelSuits::SIX); }
        if (ch == '7') { suits.push_back(CamelSuits::SEVEN); }
        if (ch == '8') { suits.push_back(CamelSuits::EIGHT); }
        if (ch == '9') { suits.push_back(CamelSuits::NINE); }
        if (ch == 'T') { suits.push_back(CamelSuits::TEN); }
        if (ch == 'J') { suits.push_back(CamelSuits::JOKER); }
        if (ch == 'Q') { suits.push_back(CamelSuits::QUEEN); }
        if (ch == 'K') { suits.push_back(CamelSuits::KING); }
        if (ch == 'A') { suits.push_back(CamelSuits::ACE); }
    }
    return suits;
}
struct camelGame {
    std::vector<Hand> hands;

    void addHand(int s1, int s2, int s3, int s4, int s5, int bid) {
        Hand hand {};

        hand.hand[0] = s1;
        hand.hand[1] = s2;
        hand.hand[2] = s3;
        hand.hand[3] = s4;
        hand.hand[4] = s5;
        hand.bid = bid;

        hands.push_back(hand);
    }

    /**
     * Sort the hands in the game by strength
     */
    void sortHands() { hands = mergeSort(hands); }

    /**
     * Sorting helpers
     */

    void copyHand(Hand& a, const Hand b) {
        a.hand[0] = b.hand[0];
        a.hand[1] = b.hand[1];
        a.hand[2] = b.hand[2];
        a.hand[3] = b.hand[3];
        a.hand[4] = b.hand[4];
        a.bid = b.bid;
    }

    std::vector<Hand> splitVector(int from, int to,
                                  const std::vector<Hand> vec) {
        std::vector<Hand> result;
        // std::cout << "from: " << from << " to: " << to << "\n";
        for (int i = from; i <= to; i++) { result.push_back(vec[i]); }

        return result;
    }

    std::vector<Hand> merge(const std::vector<Hand>& left,
                            const std::vector<Hand>& right) {
        std::vector<Hand> result;

        int indexLeft = 0;
        int indexRight = 0;
        bool merging = true;

        bool mergeRightOnly = false;
        bool mergeLeftOnly = false;
        while (merging) {
            if (mergeLeftOnly) {
                Hand leftHand {};
                copyHand(leftHand, left[indexLeft]);

                result.push_back(leftHand);
                indexLeft++;
            } else if (mergeRightOnly) {
                Hand rightHand {};
                copyHand(rightHand, right[indexRight]);

                result.push_back(rightHand);
                indexRight++;
            } else {
                Hand leftHand {};
                copyHand(leftHand, left[indexLeft]);

                Hand rightHand {};
                copyHand(rightHand, right[indexRight]);

                // not the bool for descending order
                if (!isStronger(leftHand, rightHand)) {
                    result.push_back(leftHand);
                    indexLeft++;
                } else {
                    result.push_back(rightHand);
                    indexRight++;
                }
            }

            if (indexRight >= right.size()) mergeLeftOnly = true;
            if (indexLeft >= left.size()) mergeRightOnly = true;
            if (indexLeft >= left.size() && indexRight >= right.size())
                merging = false;
        }

        return result;
    }

    std::vector<Hand> mergeSort(std::vector<Hand> hands) {
        if (hands.size() == 1) return hands;

        int mid = hands.size() / 2;
        int end = hands.size() - 1;

        if (hands.size() == 2) mid = 0;

        std::vector<Hand> left = mergeSort(splitVector(0, mid, hands));
        std::vector<Hand> right = mergeSort(splitVector(mid + 1, end, hands));

        return merge(left, right);
    }
};

std::string handToString(Hand hand) {
    std::string handStr;
    for (int i = 0; i < 5; i++) {
        if (hand.hand[i] == 2) handStr.push_back('2');
        else if (hand.hand[i] == 3)
            handStr.push_back('3');
        else if (hand.hand[i] == 4)
            handStr.push_back('4');
        else if (hand.hand[i] == 5)
            handStr.push_back('5');
        else if (hand.hand[i] == 6)
            handStr.push_back('6');
        else if (hand.hand[i] == 7)
            handStr.push_back('7');
        else if (hand.hand[i] == 8)
            handStr.push_back('8');
        else if (hand.hand[i] == 9)
            handStr.push_back('9');
        else if (hand.hand[i] == 10)
            handStr.push_back('T');
        else if (hand.hand[i] == 11)
            handStr.push_back('J');
        else if (hand.hand[i] == 12)
            handStr.push_back('Q');
        else if (hand.hand[i] == 13)
            handStr.push_back('K');
        else
            handStr.push_back('A');
    }

    return handStr;
}

/**
 * Part 1
 */
void printHands(const camelGame& game) {
    for (const auto& hand : game.hands)
        std::cout << "Hand: " << handToString(hand) << " Bid: " << hand.bid
                  << "\n";
}

void part1(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(std::move(inputFile));

    isPart1 = true;
    camelGame game;

    std::string line;
    while (std::getline(*file, line)) {
        std::vector<std::string> tokens = tokenize(line);
        std::vector<CamelSuits> suits = stringToHand(tokens[0]);
        game.addHand(suits[0], suits[1], suits[2], suits[3], suits[4],
                     std::stoi(tokens[1]));
    }

    game.sortHands();

    int rank = 1;
    int result = 0;
    for (auto& hand : game.hands) {
        result += hand.bid * rank;
        rank++;
    }

    std::cout << "Part 1: Added result is " << result << "\n";
}

/**
 * Part 2
 */
void part2(std::string inputFile) {
    std::shared_ptr<std::ifstream> file = openFile(std::move(inputFile));
    isPart1 = false;

    camelGame game;

    std::string line;
    while (std::getline(*file, line)) {
        std::vector<std::string> tokens = tokenize(line);
        std::vector<CamelSuits> suits = stringToHand(tokens[0]);
        game.addHand(suits[0], suits[1], suits[2], suits[3], suits[4],
                     std::stoi(tokens[1]));
    }

    game.sortHands();

    int rank = 1;
    int result = 0;
    for (auto& hand : game.hands) {
        result += hand.bid * rank;
        rank++;
    }

    std::cout << "Part 2: Added result is " << result << "\n";
}

/**
 *--- Day 7: Camel Cards ---
 *
 * Your all-expenses-paid trip turns out to be a one-way, five-minute ride in
 *an airship. (At least it's a cool airship!) It drops you off at the edge of
 *a vast desert and descends back to Island Island.
 *
 * "Did you bring the parts?"
 *
 * You turn around to see an Elf completely covered in white clothing, wearing
 * goggles, and riding a large camel.
 *
 * "Did you bring the parts?" she asks again, louder this time. You aren't
 *sure what parts she's looking for; you're here to figure out why the sand
 *stopped.
 *
 * "The parts! For the sand, yes! Come with me; I will show you." She beckons
 *  you onto the camel.
 *
 * After riding a bit across the sands of Desert Island, you can see what look
 * like very large rocks covering half of the horizon. The Elf explains that
 *the rocks are all along the part of Desert Island that is directly above
 *Island Island, making it hard to even get there. Normally, they use big
 *machines to move the rocks and filter the sand, but the machines have broken
 *down because Desert Island recently stopped receiving the parts they need to
 *fix the machines.
 *
 * You've already assumed it'll be your job to figure out why the parts
 *stopped when she asks if you can help. You agree automatically.
 *
 * Because the journey will take a few days, she offers to teach you the game
 *of Camel Cards. Camel Cards is sort of similar to poker except it's designed
 *to be easier to play while riding a camel.
 *
 * In Camel Cards, you get a list of hands, and your goal is to order them
 *based on the strength of each hand. A hand consists of five cards labeled
 *one of A, K, Q, J, T, 9, 8, 7, 6, 5, 4, 3, or 2. The relative strength of
 *each card follows this order, where A is the highest and 2 is the lowest.
 *
 * Every hand is exactly one type. From strongest to weakest, they are:
 *
 * Five of a kind, where all five cards have the same label: AAAAA
 * Four of a kind, where four cards have the same label and one card has a
 * different label: AA8AA Full house, where three cards have the same label,
 *and the remaining two cards share a different label: 23332 Three of a kind,
 *where three cards have the same label, and the remaining two cards are each
 * different from any other card in the hand: TTT98 Two pair, where two cards
 * share one label, two other cards share a second label, and the remaining
 *card has a third label: 23432 One pair, where two cards share one label, and
 *the other three cards have a different label from the pair and each other:
 *A23A4 High card, where all cards' labels are distinct: 23456
 *
 * Hands are primarily ordered based on type; for example, every full house is
 * stronger than any three of a kind.
 *
 * If two hands have the same type, a second ordering rule takes effect. Start
 * by comparing the first card in each hand. If these cards are different, the
 * hand with the stronger first card is considered stronger. If the first card
 * in each hand have the same label, however, then move on to considering the
 * second card in each hand. If they differ, the hand with the higher second
 * card wins; otherwise, continue with the third card in each hand, then the
 * fourth, then the fifth.
 *
 * So, 33332 and 2AAAA are both four of a kind hands, but 33332 is stronger
 * because its first card is stronger. Similarly, 77888 and 77788 are both a
 * full house, but 77888 is stronger because its third card is stronger (and
 * both hands have the same first and second card).
 *
 * To play Camel Cards, you are given a list of hands and their corresponding
 * bid (your puzzle input). For example:
 *
 * 32T3K 765
 * T55J5 684
 * KK677 28
 * KTJJT 220
 * QQQJA 483
 *
 * This example shows five hands; each hand is followed by its bid amount.
 * Each hand wins an amount equal to its bid multiplied by its rank, where the
 * weakest hand gets rank 1, the second-weakest hand gets rank 2, and so on up
 * to the strongest hand. Because there are five hands in this example, the
 * strongest hand will have rank 5 and its bid will be multiplied by 5.
 *
 * So, the first step is to put the hands in order of strength:
 *
 * 32T3K is the only one pair and the other hands are all a stronger type,
 * so it gets rank 1. KK677 and KTJJT are both two pair. Their first cards
 * both have the same label, but the second card of KK677 is stronger (K vs T),
 * so KTJJT gets rank 2 and KK677 gets rank 3. T55J5 and QQQJA are both three
 * of a kind. QQQJA has a stronger first card, so it gets rank 5 and T55J5 gets
 * rank 4.
 *
 * Now, you can determine the total winnings of this set of hands by adding up
 * the result of multiplying each hand's bid with its rank (765 * 1 + 220 * 2
 *+ 28 * 3 + 684 * 4 + 483 * 5). So the total winnings in this example are
 * 6440.
 *
 * Find the rank of every hand in your set. What are the total winnings?
 *
 *                              --- Part Two ---
 * To make things a little more interesting, the Elf introduces one additional
 * rule. Now, J cards are jokers - wildcards that can act like whatever card
 * would make the hand the strongest type possible.
 *
 * To balance this, J cards are now the weakest individual cards, weaker even
 * than 2. The other cards stay in the same order: A, K, Q, T, 9, 8, 7, 6, 5, 4,
 * 3, 2, J.
 *
 * J cards can pretend to be whatever card is best for the purpose of
 * determining hand type; for example, QJJQ2 is now considered four of a kind.
 * However, for the purpose of breaking ties between two hands of the same type,
 * J is always treated as J, not the card it's pretending to be: JKKK2 is weaker
 * than QQQQ2 because J is weaker than Q.
 *
 * Now, the above example goes very differently:
 *
 * 32T3K 765
 * T55J5 684
 * KK677 28
 * KTJJT 220
 * QQQJA 483
 *
 * 32T3K is still the only one pair; it doesn't contain any jokers, so its
 * strength doesn't increase. KK677 is now the only two pair, making it the
 * second-weakest hand. T55J5, KTJJT, and QQQJA are now all four of a kind!
 * T55J5 gets rank 3, QQQJA gets rank 4, and KTJJT gets rank 5.
 *
 * With the new joker rule, the total winnings in this example are 5905.
 *
 * Using the new joker rule, find the rank of every hand in your set. What are
 *the new total winnings?
 *
 */
