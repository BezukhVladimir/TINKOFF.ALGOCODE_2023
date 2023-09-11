#include <ctime>
#include <vector>
#include <random>
#include <sstream>
#include <iostream>
#include <functional>

// https://onlinegdb.com/ePUu60Y4O simulated mode

class Blackjack {
private:
    double ALPHA               = 0.001;  // reward bonus
    double EPSILON             = 0.99;   // probability bonus
    double EPSILON_DECAY       = 0.9999; // reduced impact
    double EPSILON_MINIMUM     = 0.5;    // epsilon lower limit
    double GAMMA               = 0.8;    // discount rate
    double START_PROBABILITY   = 0.33;   // hit, double or break
    int    TRAINING_ROUNDS     = 500;    // number of rounds with small bets
    int    TRAINING_BET_SIZE   = 1000;   // small bet size
    double PERCENTAGE_OF_BANK  = 0.021;  // common bet size
    double LAST_CHANCE_ROUNDS  = 21;      // number of last chances
    double LAST_CHANCE_PERCENT = 0.21;   // last chance bet size

    static constexpr int BLACKJACK             = 21;
    static constexpr int DEALER_HAND_LIMIT     = 17;
    static constexpr int MINIMUM_DECK_SIZE     = 26;
    static constexpr int WIN_LIMIT             = 4200000;
    static constexpr int GAMEPLAY_ROUNDS_LIMIT = 9999;
    static constexpr int CARD_TYPES            = 13;
    static constexpr int CARD_COPIES           = 4;

    enum CardType {
        _2, _3, _4, _5, _6, _7, _8, _9,
        _0, _J, _Q, _K, _A
    };

    static constexpr int CARD_VALUES[] = {
            2, 3, 4, 5, 6, 7, 8, 9,
            10, 10, 10,10, 11
    };

    CardType readCardType(const char& card) const {
        if ('2' <= card && card <= '9') {
            return (CardType)(card - '2');
        } else if (card == '0') {
            return (CardType)(8);
        } else if (card == 'J') {
            return (CardType)(9);
        } else if (card == 'Q') {
            return (CardType)(10);
        } else if (card == 'K') {
            return (CardType)(11);
        } else if (card == 'A') {
            return (CardType)(12);
        } else {
            return (CardType)(0); // error
        }
    }

    char getChar(const CardType& type) const {
        char c;
        switch (type) {
            case _2: c = '2'; break;
            case _3: c = '3'; break;
            case _4: c = '4'; break;
            case _5: c = '5'; break;
            case _6: c = '6'; break;
            case _7: c = '7'; break;
            case _8: c = '8'; break;
            case _9: c = '9'; break;
            case _0: c = '0'; break;
            case _J: c = 'J'; break;
            case _Q: c = 'Q'; break;
            case _K: c = 'K'; break;
            case _A: c = 'A'; break;
        }

        return c;
    }

    class Deck {
    private:
        int size;
        std::vector<int> card_counters;

    public:
        Deck() {
            size = CARD_TYPES * CARD_COPIES;
            card_counters.resize(CARD_TYPES, CARD_COPIES);
        }

        int howMuch(const CardType& type) const {
            return card_counters[type];
        }

        int getSize() {
            return size;
        }

        void discard(const CardType& type) {
            --size;
            --card_counters[type];
        }

        void reset() {
            size = CARD_TYPES * CARD_COPIES;
            std::fill(card_counters.begin(), card_counters.end(), CARD_COPIES);
        }
    };

    class Hand {
    private:
        int ace_number;
        int size;
        int base;
        int power;

    public:
        Hand() {
            clear();
        }

        bool operator==(const Hand& other) const {
            return (power == other.power)
                   && (ace_number == other.ace_number);
        }

        void add(const CardType& card) {
            ++size;

            if (card != _A) {
                base += CARD_VALUES[card];
            } else {
                ++ace_number;
            }

            if (ace_number != 0) {
                int t = base + CARD_VALUES[_A] + (ace_number - 1);
                if (t <= BLACKJACK) {
                    power = t;
                } else {
                    power = base + ace_number;
                }
            } else {
                power = base;
            }
        }

        int getSize() const {
            return size;
        }

        int getPower() const {
            return power;
        }

        int getAceNumber() const {
            return ace_number;
        }

        void clear() {
            ace_number = 0;
            size  = 0;
            base  = 0;
            power = 0;
        }
    };

    struct HandHasher {
        std::size_t operator()(const Hand& h) const noexcept {
            size_t res = 17;
            res = res * 31 + std::hash<int>()(h.getPower());
            res = res * 31 + std::hash<int>()(h.getAceNumber());
            return res;
        }
    };

    class State {
    public:
        Hand dealer;
        Hand player;

        State() {
            clear();
        }

        bool operator==(const State& other) const {
            return (player == other.player)
                   && (dealer == other.dealer);
        }

        void clear() {
            dealer.clear();
            player.clear();
        }

        int getPlayerPower() const {
            return player.getPower();
        }

        int getDealerPower() const {
            return dealer.getPower();
        }
    };

    struct StateHasher {
        std::size_t operator()(const State& s) const noexcept {
            size_t res = 17;
            res = res * 31 + HandHasher()(s.dealer);
            res = res * 31 + HandHasher()(s.player);
            return res;
        }
    };

    enum ActionType {
        _hit,
        _double,
        _break
    };

    struct StateAction {
        State state;
        ActionType action;

        StateAction(const State& state_, const ActionType& action_) {
            state  = state_;
            action = action_;
        }

        bool operator==(const StateAction& other) const {
            return (state  == other.state) &&
                   (action == other.action);
        }
    };

    struct StateActionHasher {
        std::size_t operator()(const StateAction& s) const noexcept {
            size_t res = 17;
            res = res * 31 + StateHasher()(s.state);
            res = res * 31 + std::hash<int>()(s.action);
            return res;
        }
    };

    double randReal(const double &min = 0, const double &max = 1) const {
        static std::mt19937 gen(std::time(NULL));
        std::uniform_real_distribution<double> uid(min, max);
        return uid(gen);
    }

    int randNumber(const int &min = 0, const int &max = RAND_MAX) const {
        static std::mt19937 gen(std::time(NULL));
        std::uniform_int_distribution<int> uid(min, max);
        return uid(gen);
    }

    std::string getCommandSimulated() {
        if (deck.getSize() < MINIMUM_DECK_SIZE) {
            std::cout << "shuffle" << std::endl;
            return "shuffle";
        } else {
            std::cout << "bet" << std::endl;
            return "bet";
        }
    }

    std::string getCommand() {
        std::string command;
        std::getline(std::cin, command);
        return command;
        // return getCommandSimulated();
    }

    void sendCommand(const std::string& command) {
        std::cout << command << std::endl;
    }

    void bet(const int& size) {
        sendCommand(std::to_string(size));
        current_bet += size;
        player_bank -= size;
    }

    bool willingToBet() {
        int bet_size;
        if (rounds_played < TRAINING_ROUNDS) {
            bet_size = TRAINING_BET_SIZE;
        } else if (rounds_played >= GAMEPLAY_ROUNDS_LIMIT - LAST_CHANCE_ROUNDS) {
            bet_size = player_bank * LAST_CHANCE_PERCENT;
        }  else {
            bet_size = player_bank * PERCENTAGE_OF_BANK;
        }

        if (bet_size <= player_bank
            && player_bank < WIN_LIMIT
            && rounds_played < GAMEPLAY_ROUNDS_LIMIT) {
            bet(bet_size);
            return true;
        } else {
            sendCommand("-1");
            // std::cout << player_bank << std::endl;
            return false;
        }
    }

    int getFirstNewCardIndex(const Hand& hand, const std::string& cards) const {
        return hand.getSize();
    }

    void addCards(Hand& hand,
                  const std::string& cards,
                  const int& first_new_card_index) {
        for (int i = first_new_card_index; i < cards.size(); ++i) {
            CardType current_card = readCardType(cards[i]);
            hand.add(current_card);
            deck.discard(current_card);
        }
    }

    CardType getRandomCardType() const {
        int rnd = randNumber(0, 12);
        while (deck.howMuch((CardType) rnd) <= 0) {
            rnd = randNumber(0, 12);
        }

        return (CardType) rnd;
    }

    void readCardSimulated(std::string& cards) {
        CardType rnd = getRandomCardType();
        deck.discard(rnd);
        char c = getChar(rnd);
        cards += c;
    }

    void readCardsSimulated(State& state,
                            std::string& dealer_cards,
                            std::string& player_cards) {
        if (state.player.getSize() == 0) {
            readCardSimulated(dealer_cards);
            readCardSimulated(player_cards);
            readCardSimulated(player_cards);
        } else {
            player_cards.resize(state.player.getSize(), '_'); // previous characters
            dealer_cards.resize(state.dealer.getSize(), '_'); // are ignored
            if (current_round_moves[current_round_moves.size() - 1].action == _break) {
                readCardSimulated(dealer_cards);
            } else {
                readCardSimulated(player_cards);
            }
        }

        std::cout << dealer_cards << '#' << player_cards << std::endl;
    }

    void readCards(std::string& dealer_cards,
                   std::string& player_cards) {
        std::string table_state;
        std::getline(std::cin, table_state);
        std::stringstream ss(table_state);
        std::getline(ss, dealer_cards, '#');
        std::getline(ss, player_cards);
    }

    void updateState(State& state,
                     const std::string& dealer_cards,
                     const std::string& player_cards) {
        int first_new_card_index;
        first_new_card_index = getFirstNewCardIndex(state.dealer, dealer_cards);
        addCards(state.dealer, dealer_cards, first_new_card_index);

        first_new_card_index = getFirstNewCardIndex(state.player, player_cards);
        addCards(state.player, player_cards, first_new_card_index);
    }

    void update(State& state) {
        std::string dealer_cards;
        std::string player_cards;

        readCards(dealer_cards, player_cards);
        // readCardsSimulated(state, dealer_cards, player_cards);

        updateState(state, dealer_cards, player_cards);
    }

    std::vector<ActionType> getActionRating(State& state) {
        StateAction sa_hit(state, _hit);
        StateAction sa_double(state, _double);
        StateAction sa_break(state, _break);

        updateRewardsStateActionTable(sa_hit, 0.0);
        updateRewardsStateActionTable(sa_double, 0.0);
        updateRewardsStateActionTable(sa_break, 0.0);

        std::vector<double> scores(3);
        std::vector<ActionType> result = { _hit, _double, _break };
        scores[_hit]    = rewards[sa_hit];
        scores[_double] = rewards[sa_double];
        scores[_break]  = rewards[sa_break];

        if (scores[_hit] < scores[_double]) {
            std::swap(scores[_hit], scores[_double]);
            std::swap(result[_hit], result[_double]);
        }

        if (scores[_hit] < scores[_break]) {
            std::swap(scores[_hit], scores[_break]);
            std::swap(result[_hit], result[_break]);
        }

        if (scores[_double] < scores[_break]) {
            std::swap(scores[_double], scores[_break]);
            std::swap(result[_double], result[_break]);
        }

        return result;
    }

    std::vector<double> getActionChances(State& state) {
        StateAction sa_hit(state, _hit);
        StateAction sa_double(state, _double);
        StateAction sa_break(state, _break);

        updateProbabilitiesStateActionTable(sa_hit, 0.0);
        updateProbabilitiesStateActionTable(sa_double, 0.0);
        updateProbabilitiesStateActionTable(sa_break, 0.0);

        std::vector<double> result(3);
        result[_hit]    = probabilities[sa_hit];
        result[_double] = probabilities[sa_double];
        result[_break]  = probabilities[sa_break];

        return result;
    }

    void sendDouble(State& state) {
        player_bank -= current_bet;
        current_bet *= 2;
        sendCommand("double");
        StateAction move(state, _double);
        current_round_moves.emplace_back(move);
        update(state);
    }

    void sendHit(State& state) {
        sendCommand("hit");
        StateAction move(state, _hit);
        current_round_moves.emplace_back(move);
        update(state);
    }

    void sendBreak(State& state) {
        sendCommand("break");
        StateAction move(state, _break);
        current_round_moves.emplace_back(move);
    }

    void sendAction(State& current_state, const ActionType& best_action) {
        if (best_action == _double) {
            if (player_bank > current_bet) {
                sendDouble(current_state);
                return;
            } else {
                sendHit(current_state);
                return;
            }
        } else if (best_action == _hit) {
            sendHit(current_state);
            return;
        }
        
        sendBreak(current_state);
    }

    // DEALER CARD
    std::vector<std::vector<ActionType>> HEURISTIC_TABLE = {
            { _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit   }, // -8  P
            { _hit,    _hit,    _hit,    _hit,    _double, _hit,    _hit,    _hit,    _hit,    _hit   }, //  9  L
            { _double, _double, _double, _double, _double, _double, _double, _hit,    _hit,    _hit   }, // 10  A
            { _double, _double, _double, _double, _double, _double, _double, _hit,    _hit,    _hit   }, // 11  Y
            { _hit,    _hit,    _hit,    _break,  _break,  _hit,    _hit,    _hit,    _hit,    _hit   }, // 12  E
            { _hit,    _break,  _break,  _break,  _break,  _hit,    _hit,    _hit,    _hit,    _hit   }, // 13  R
            { _hit,    _break,  _break,  _break,  _break,  _hit,    _hit,    _hit,    _hit,    _hit   }, // 14
            { _break,  _break,  _break,  _break,  _break,  _hit,    _hit,    _hit,    _hit,    _hit   }, // 15
            { _break,  _break,  _break,  _break,  _break,  _hit,    _hit,    _hit,    _hit,    _hit   }, // 16
            { _break,  _break,  _break,  _break,  _break,  _break,  _break,  _break,  _break,  _break }, // 17+
            { _break,  _break,  _break,  _break,  _break,  _break,  _break,  _break,  _break,  _break }, // A+8
            { _break,  _break,  _break,  _double, _double, _break,  _break,  _hit,    _hit,    _hit   }, // A+7
            { _hit,    _hit,    _hit,    _double, _double, _hit,    _hit,    _hit,    _hit,    _hit   }, // A+6
            { _hit,    _hit,    _hit,    _hit,    _double, _hit,    _hit,    _hit,    _hit,    _hit   }, // A+5
            { _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit   }, // A+4 H
            { _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit   }, // A+3 A
            { _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit   }, // A+2 N
            { _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit,    _hit   }  // A+A D
    };

    void playerMoveHeuristicPlayStyle(State& current_state) {
        while (true) {
            int pl_i = current_state.getPlayerPower();
            if (pl_i > BLACKJACK) {
                pl_i = BLACKJACK;
            }

            int dl_j = current_state.getDealerPower();
            if (dl_j > 9) {
                dl_j = 9;
            }
            dl_j -= 2; // horizontal normalize

            // vertical normalize
            if (current_state.player.getAceNumber() == 0) {
                switch (pl_i) {
                    case 2: case 3: case 4: case 5:
                    case 6: case 7: case 8:
                        pl_i = 0;
                        break;
                    case  9: case 10: case 11: case 12:
                    case 13: case 14: case 15: case 16:
                        pl_i -= 8;
                        break;
                    case 17: case 18: case 19: case 20:
                    case 21:
                        pl_i = 9;
                        break;
                    default:
                        break;
                }
            } else {
                switch (pl_i) {
                    case 21: case 20: case 19:
                        pl_i = 10;
                        break;
                    case 18: pl_i = 11; break;
                    case 17: pl_i = 12; break;
                    case 16: pl_i = 13; break;
                    case 15: pl_i = 14; break;
                    case 14: pl_i = 15; break;
                    case 13: pl_i = 16; break;
                    case 12: pl_i = 17; break;
                    default:
                        break;
                }
            }

            ActionType best_action = HEURISTIC_TABLE[pl_i][dl_j];
            sendAction(current_state, best_action);
            if (current_round_moves[current_round_moves.size() - 1].action == _break) {
                return;
            }
        }
    }

    void playerMoveMonteCarloPlayStyle(State& current_state) {
        while (current_state.player.getPower() < DEALER_HAND_LIMIT) {
            std::vector<ActionType> rating  = getActionRating(current_state);
            std::vector<double>     chances = getActionChances(current_state);

            ActionType best_action;
            double choice = randReal();
            for (const auto& action : rating) {
                if (choice <= chances[action]) {
                    best_action = action;
                    break;
                }

                choice -= chances[action];
            }

            sendAction(current_state, best_action);
            if (current_round_moves[current_round_moves.size() - 1].action == _break) {
                return;
            }
        }

        sendBreak(current_state);
        if (current_round_moves[current_round_moves.size() - 1].action == _break) {
            return;
        }
    }

    void playerMove(State& current_state) {
        playerMoveHeuristicPlayStyle(current_state);
        // playerMoveMonteCarloPlayStyle(current_state);
    }

    void dealerMove(State& current_state) {
        while (current_state.getDealerPower() < DEALER_HAND_LIMIT) {
            update(current_state);
        }
    }

    void updateRewards(const int& value) {
        double reward = value * ALPHA;
        for (int i = current_round_moves.size() - 1; i >= 0; --i) {
            updateRewardsStateActionTable(current_round_moves[i], reward);
            reward *= GAMMA;
        }
    }

    void updateRewardsStateActionTable(const StateAction& key,
                                       const double& value) {
        auto it = rewards.find(key);
        if (it != rewards.end()) {
            rewards[key] += value;
        } else {
            rewards[key] = value;
        }
    }

    void updateProbabilities(const bool& isWin) {
        double probability = (1.0 - EPSILON);
        probability *= (isWin) ? 1.0 : -1.0;

        for (int i = current_round_moves.size() - 1; i >= 0; --i) {
            StateAction move = current_round_moves[i];
            ActionType  current = move.action;

            // Sum of the probabilities for one state may be higher than 1,
            // this is an error that can be corrected
            if (current == _hit) {
                // reward
                updateProbabilitiesStateActionTable(move, probability);
                move.action = _double;
                updateProbabilitiesStateActionTable(move, 0.5 * probability);

                // punish
                move.action = _break;
                updateProbabilitiesStateActionTable(move, -1.5 * probability);
            } else if (current == _double) {
                // reward
                updateProbabilitiesStateActionTable(move, probability);
                move.action = _hit;
                updateProbabilitiesStateActionTable(move, 1.5 * probability);

                // punish
                move.action = _break;
                updateProbabilitiesStateActionTable(move, -2.5 * probability);
            } else {
                // reward
                updateProbabilitiesStateActionTable(move, 2.5 * probability);

                // punish
                move.action = _hit;
                updateProbabilitiesStateActionTable(move, -probability);
                move.action = _double;
                updateProbabilitiesStateActionTable(move, -1.5 * probability);
            }

            probability *= GAMMA;
        }
    }

    void updateProbabilitiesStateActionTable(const StateAction& key,
                                             const double& value) {
        auto it = probabilities.find(key);
        if (it != probabilities.end()) {
            probabilities[key] += value;
        } else {
            probabilities[key] = START_PROBABILITY + value;
        }

        if (probabilities[key] > 1.0) {
            probabilities[key] = 1.0;
        }

        if (probabilities[key] < 0.0) {
            probabilities[key] = 0.0;
        }
    }

    void sumUpResultsOfGameRound(const State& current_state) {
        int player_score = current_state.player.getPower();
        int dealer_score = current_state.dealer.getPower();

        if (player_score > BLACKJACK) {
            player_score = 0;
        }

        if (dealer_score > BLACKJACK) {
            dealer_score = 0;
        }

        int prize = 0;
        if (player_score > dealer_score) {
            // MonteCarloPlayStyle
            // updateRewards(current_bet);
            // updateProbabilities(true);
            prize = current_bet * 2;
        } else if (player_score == dealer_score) {
            prize = current_bet;
        } else {
            // MonteCarloPlayStyle
            // updateRewards(-current_bet);
            // updateProbabilities(false);
        }

        player_bank += prize;
        current_bet = 0;
        current_round_moves.clear();
        
        if (EPSILON > EPSILON_MINIMUM) {
            EPSILON *= EPSILON_DECAY;
        }

        ++rounds_played;
    }

    Deck deck;
    int  current_bet;
    int  player_bank;

    int rounds_played;
    typedef std::unordered_map<StateAction, double, StateActionHasher> T_StateActionTable;
    T_StateActionTable rewards;
    T_StateActionTable probabilities;
    std::vector<StateAction> current_round_moves;
    
public:
    Blackjack() {
        rounds_played = 0;
        current_bet   = 0;
        player_bank   = 500000;
    }

    void play() {
        State current_state;
        std::string current_command;

        while (true) {
            current_command = getCommand();

            if (current_command == "bet") {
                if (willingToBet()) {
                    update(current_state);
                } else {
                    break;
                }
            } else if (current_command == "shuffle") {
                deck.reset();
                continue;
            }

            playerMove(current_state);
            dealerMove(current_state);
            sumUpResultsOfGameRound(current_state);
            current_state.clear();
        }
    }
};

int main() {
    std::cin.tie(NULL);
    std::ios_base::sync_with_stdio(false);

    Blackjack game;
    game.play();
}