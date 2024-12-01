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

// �ֽ� ������ �������� �����ϴ� �Լ�
vector<int> generateStockPrices(int days) {
    vector<int> prices(days);
    
    random_device rd;  // ���� ��ġ
    mt19937 gen(rd());  // ���� ������ (Mersenne Twister �˰���)
    
    // ù�� �ֽ� ���� ����
    uniform_int_distribution<int> dist(1, 100);  // 1���� 100������ �յ����
    prices[0] = dist(gen);
    
    // �ֽ� ���� ���� ���� (80% ����)
    for (int i = 1; i < days; ++i) {
        uniform_int_distribution<int> changeDist(-80, 80);  // -80%���� +80%������ ���� ����
        int change = changeDist(gen);  // ������ ���
        
        // ���� ���ݿ� ������ �����Ͽ� �� ���� ��� (�ּ� ������ 1�� ����)
        prices[i] = max(1, prices[i - 1] + (prices[i - 1] * change / 100));
    }
    return prices;
}

// �� ���� �ŷ��� �ִ� ������ ����ϴ� �Լ�
TSResult maxProfitWithTwoTransactions(const vector<int>& prices, int days) {
	TSResult res;
    int maxProfit = 0;

    // ������ (3�Ϻ��� days-1���� ����)
    for (int mid = 3; mid < days-1; ++mid) {
        // ù ��° ���� (�ż�/�ŵ�): 0���� mid-1����
        int minPrice1[2] = {prices[0], 0};
        int maxProfit1 = 0;
        int buyDay1 = 0, sellDay1 = 0; 
        
        for (int i = 1; i < mid; ++i) { // �ι�°������ 
        	// �Ⱦ��� ���� �̵� ����ϱ� 
			int profit = prices[i] - minPrice1[0];
        	
            // �ִ� ����(+��¥) ���� 
        	if(maxProfit1 < profit) {
        		maxProfit1 = profit;
        		buyDay1 = minPrice1[1];
        		sellDay1 = i;
			}

        	// �ּ� ����(+��¥) ���� 
        	if(minPrice1[0] < prices[i]) {
        		minPrice1[0] = prices[i];
        		minPrice1[1] = i;
			}
        }

        // �� ��° ���� (�ż�/�ŵ�): mid���� days-1����
        int minPrice2[2] = {prices[mid], mid};
        int maxProfit2 = 0;
        int buyDay2 = mid, sellDay2 = mid;

        for (int i = mid + 1; i < days; ++i) {
            // �Ⱦ��� ���� �̵� ����ϱ� 
			int profit = prices[i] - minPrice2[0];
        	
            // �ִ� ����(+��¥) ���� 
        	if(maxProfit2 < profit) {
        		maxProfit2 = profit;
        		buyDay2 = minPrice2[1];
        		sellDay2 = i;
			}

        	// �ּ� ����(+��¥) ���� 
        	if(minPrice2[0] < prices[i]) {
        		minPrice2[0] = prices[i];
        		minPrice2[1] = i;
			}
        }

        // �� ���� ��� (�� �ŷ��� �ջ�)
        int totalProfit = maxProfit1 + maxProfit2;

        // �ִ� ������ ���
        if (totalProfit > maxProfit) {
            maxProfit = totalProfit;
            
            res.set(maxProfit, buyDay1, sellDay1, buyDay2, sellDay2);
        }
    }
    
    return res;
}

int main() {
    int days = -1;  // �ùķ��̼��� �ϼ�
    
    // �� �� �Է� 
    cout << "�ùķ��̼� �� �ϼ��� �Է��ϼ��� : ";
    cin >> days;
    vector<int> prices = generateStockPrices(days);

    // �ִ� ���� ��� (�� ���� �ŷ�)
    TSResult result = maxProfitWithTwoTransactions(prices, days);

    // ��� ���
    cout << "�ֽ� ���� (�Ϻ�): ";
    for (int i = 0; i < days; ++i) {
        cout << prices[i] << " ";
    }
    cout << endl;

    cout << "�ִ� ����: " << result.maxProfit << "$\n";
    cout << "�ŷ� ���� (���� ��, �Ⱦ��� ��) : \n";
    for (int i=0; i<2; i++) {
        cout << "������: " << result.buyDay[i] + 1 << "�� (" << prices[result.buyDay[i]] 
             << "$) �Ǹ���: " << result.sellDay[i] + 1 << "��(" << prices[result.sellDay[i]]
             << "$), �̵�: " << prices[result.sellDay[i]] - prices[result.buyDay[i]] << "$\n";
    }

    return 0;
}

