#include "Common.h"
#include "test_runner.h"

#include <sstream>
#include "Common.h"

using namespace std;

string Print(const Expression* e) {
    if (!e) {
        return "Null expression provided";
    }
    stringstream output;
    output << e->ToString() << " = " << e->Evaluate();
    return output.str();
}

void Test() {
    ExpressionPtr e1 = Product(Value(2), Sum(Value(3), Value(4)));
    ASSERT_EQUAL(Print(e1.get()), "(2)*((3)+(4)) = 14");

    ExpressionPtr e2 = Sum(move(e1), Value(5));
    ASSERT_EQUAL(Print(e2.get()), "((2)*((3)+(4)))+(5) = 19");

    ASSERT_EQUAL(Print(e1.get()), "Null expression provided");
}

class Constant : public Expression {
public:
	Constant(int v) : value_(v)
	{}
	int Evaluate() const {
		return value_;
	}

	std::string ToString() const {
		std::string result = std::to_string(value_);
		return result;
	}
private:
	int value_;
};

class Operation : public Expression {
public:
	Operation(std::unique_ptr<Expression> l, std::unique_ptr<Expression> r, char o)
		: left_(std::move(l)), right_(std::move(r)), operation_(o)
	{}

	int Evaluate() const {
		int result = 0;
		switch (operation_) {
		case '+':
			result = left_->Evaluate() + right_->Evaluate();
			break;
		case '*':
			result = left_->Evaluate() * right_->Evaluate();
			break;
		}
		return result;
	}

	std::string ToString() const {
		std::string result = "(" + left_->ToString() + ")" + operation_ + "(" + right_->ToString() + ")";
		return result;
	}
private:
	char operation_;
	ExpressionPtr left_;
	ExpressionPtr right_;
};
ExpressionPtr Value(int value) {
    return std::make_unique<Constant>(Constant(value));
}
ExpressionPtr Sum(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Operation>(Operation(move(left), move(right), '+'));
}
ExpressionPtr Product(ExpressionPtr left, ExpressionPtr right) {
    return std::make_unique<Operation>(Operation(move(left), move(right), '*'));
}


int main() {
    TestRunner tr;
    RUN_TEST(tr, Test);
    return 0;
}