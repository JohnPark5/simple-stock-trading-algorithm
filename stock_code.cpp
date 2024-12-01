#include <iostream>
#include <vector>
#include <random>
#include <ctime>
#include <algorithm>
#define MAX_INT 2147483647

using namespace std;

struct TSResult {
	int maxProfit;
	int buyDay[2];
	int sellDay[2];
	void set(int maxProfit, int buyDay1, int sellDay1, int buyDay2, int sellDay2) {
		this->maxProfit = maxProfit;
		this->buyDay[0] = buyDay1;
		this->sellDay[0] = sellDay1;
		this->buyDay[1] = buyDay2;
		this->sellDay[1] = sellDay2;
	}
};

// 주식 가격을 랜덤으로 생성하는 함수
vector<int> generateStockPrices(int days) {
    vector<int> prices(days);
    
    random_device rd;  // 랜덤 장치
    mt19937 gen(rd());  // 난수 생성기 (Mersenne Twister 알고리즘)
    
    // 첫날 주식 가격 설정
    uniform_int_distribution<int> dist(1, 100);  // 1부터 100까지의 균등분포
    prices[0] = dist(gen);
    
    // 주식 가격 변동 범위 (80% 증감)
    for (int i = 1; i < days; ++i) {
        uniform_int_distribution<int> changeDist(-80, 80);  // -80%에서 +80%까지의 증감 범위
        int change = changeDist(gen);  // 증감율 계산
        
        // 이전 가격에 증감을 적용하여 새 가격 계산 (최소 가격은 1로 설정)
        prices[i] = max(1, prices[i - 1] + (prices[i - 1] * change / 100));
    }
    return prices;
}

// 두 번의 거래로 최대 이익을 계산하는 함수
TSResult maxProfitWithTwoTransactions(const vector<int>& prices, int days) {
	TSResult res;
    int maxProfit = 0;

    // 기준일 (3일부터 days-1까지 설정)
    for (int mid = 3; mid < days-1; ++mid) {
        // 첫 번째 구간 (매수/매도): 0부터 mid-1까지
        int minPrice1[2] = {prices[0], 0};
        int maxProfit1 = 0;
        int buyDay1 = 0, sellDay1 = 0; 
        
        for (int i = 1; i < mid; ++i) { // 두번째날부터 
        	// 팔았을 때의 이득 계산하기 
			int profit = prices[i] - minPrice1[0];
        	
            // 최대 이익(+날짜) 갱신 
        	if(maxProfit1 < profit) {
        		maxProfit1 = profit;
        		buyDay1 = minPrice1[1];
        		sellDay1 = i;
			}

        	// 최소 가격(+날짜) 갱신 
        	if(minPrice1[0] < prices[i]) {
        		minPrice1[0] = prices[i];
        		minPrice1[1] = i;
			}
        }

        // 두 번째 구간 (매수/매도): mid부터 days-1까지
        int minPrice2[2] = {prices[mid], mid};
        int maxProfit2 = 0;
        int buyDay2 = mid, sellDay2 = mid;

        for (int i = mid + 1; i < days; ++i) {
            // 팔았을 때의 이득 계산하기 
			int profit = prices[i] - minPrice2[0];
        	
            // 최대 이익(+날짜) 갱신 
        	if(maxProfit2 < profit) {
        		maxProfit2 = profit;
        		buyDay2 = minPrice2[1];
        		sellDay2 = i;
			}

        	// 최소 가격(+날짜) 갱신 
        	if(minPrice2[0] < prices[i]) {
        		minPrice2[0] = prices[i];
        		minPrice2[1] = i;
			}
        }

        // 총 이익 계산 (두 거래를 합산)
        int totalProfit = maxProfit1 + maxProfit2;

        // 최대 이익을 기록
        if (totalProfit > maxProfit) {
            maxProfit = totalProfit;
            
            res.set(maxProfit, buyDay1, sellDay1, buyDay2, sellDay2);
        }
    }
    
    return res;
}

int main() {
    int days = -1;  // 시뮬레이션할 일수
    
    // 일 수 입력 
    cout << "시뮬레이션 할 일수를 입력하세요 : ";
    cin >> days;
    vector<int> prices = generateStockPrices(days);

    // 최대 이익 계산 (두 번의 거래)
    TSResult result = maxProfitWithTwoTransactions(prices, days);

    // 결과 출력
    cout << "주식 가격 (일별): ";
    for (int i = 0; i < days; ++i) {
        cout << prices[i] << " ";
    }
    cout << endl;

    cout << "최대 이익: " << result.maxProfit << "$\n";
    cout << "거래 시점 (샀을 때, 팔았을 때) : \n";
    for (int i=0; i<2; i++) {
        cout << "구매일: " << result.buyDay[i] + 1 << "일 (" << prices[result.buyDay[i]] 
             << "$) 판매일: " << result.sellDay[i] + 1 << "일(" << prices[result.sellDay[i]]
             << "$), 이득: " << prices[result.sellDay[i]] - prices[result.buyDay[i]] << "$\n";
    }

    return 0;
}

