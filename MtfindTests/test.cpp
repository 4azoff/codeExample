#include "pch.h"
#include "../mtfind/Solution.h"

class SolutionTest : public ::testing::Test {
protected:

	Solution solution;

};

TEST_F(SolutionTest, FindMaskWhenLineIsEmpty) {
	solution.FindMask("", "?ad", 1);
	auto result = solution.GetAnswers();
	ASSERT_EQ(result->size(), 0);
}

int main(int argc, char** argv) {
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}