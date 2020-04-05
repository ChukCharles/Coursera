#include "test_runner.h"

#include <vector>
#include <numeric>

using namespace std;

// Объявляем Sentence<Token> для произвольного типа Token
// синонимом vector<Token>.
// Благодаря этому в качестве возвращаемого значения
// функции можно указать не малопонятный вектор векторов,
// а вектор предложений — vector<Sentence<Token>>.
template <typename Token>
using Sentence = vector<Token>;

// Класс Token имеет метод bool IsEndSentencePunctuation() const
template <typename Token>
vector<Sentence<Token>> SplitIntoSentences(vector<Token> tokens) {
    // Напишите реализацию функции, не копируя объекты типа Token
    vector<Sentence<Token>> result(1);
    bool end_token_found = false;
    for (size_t i = 0; i < tokens.size(); ++i ) {
        if (tokens[i].IsEndSentencePunctuation()) {
            result.back().push_back(move(tokens[i]));
            end_token_found = true;
        }
        else {
            if (!end_token_found) {
                result.back().push_back(move(tokens[i]));
            }
            else {
                result.push_back(Sentence<Token>{move(tokens[i])});
                end_token_found = false;
            }
        }
    }
    return result;
}


struct TestToken {
    string data;
    bool is_end_sentence_punctuation = false;

    bool IsEndSentencePunctuation() const {
        return is_end_sentence_punctuation;
    }
    bool operator==(const TestToken& other) const {
        return data == other.data && is_end_sentence_punctuation == other.is_end_sentence_punctuation;
    }
};

ostream& operator<<(ostream& stream, const TestToken& token) {
    return stream << token.data;
}

// Тест содержит копирования объектов класса TestToken.
// Для проверки отсутствия копирований в функции SplitIntoSentences
// необходимо написать отдельный тест.
void TestSplitting() {
    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!"} })),
        vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!"}}
            })
    );

    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true} })),
        vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!", true}}
            })
    );

    ASSERT_EQUAL(
        SplitIntoSentences(vector<TestToken>({ {"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}, {"Without"}, {"copies"}, {".", true} })),
        vector<Sentence<TestToken>>({
            {{"Split"}, {"into"}, {"sentences"}, {"!", true}, {"!", true}},
            {{"Without"}, {"copies"}, {".", true}},
            })
            );
}

int main() {
    TestRunner tr;
    RUN_TEST(tr, TestSplitting);
    //vector<string> nums{"abc", "def", "ghi"};
    ////iota(nums.begin(), nums.end(), "a");

    //vector<string> n2;
    //for (int i = 0; i < nums.size(); ++i) {
    //    n2.push_back(move(nums[i]));
    //}
    //for (auto x : n2) cout << x << " ";
    //cout << endl;
    //for (auto x : nums) cout << x << " ";
    //cout << endl;
    return 0;
}
