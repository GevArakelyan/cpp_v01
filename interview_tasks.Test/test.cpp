#include "pch.h"
#include "../interview_tasks/coin.h"

using namespace money;

TEST(TestMoneyCoinOperations, CoinSubtraction)
{
	EXPECT_EQ( (coin(2,2) - coin(1,1)) , coin(1,1));
}

TEST(TestMoneyCoinOperations, CoinAddition)
{
	EXPECT_EQ((coin(2, 2) + coin(1, 1)), coin(3, 3));
}

TEST(TestMoneyCoinOperations, CoinMultiply)
{
	coin c(2, 2);
	EXPECT_EQ( c *= 1 , coin(2, 2));
}

TEST(TestMoneyCoinOperations, CoinDevide)
{
	coin c(2, 2);
	EXPECT_EQ( c /= 2, coin(1, 1));
}