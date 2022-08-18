#include <iostream>
#include <fstream>
using namespace std;

void open_file(ifstream &file, string fileName) {
    file.open(fileName);
	if (file)
		cout << "File " << fileName << " open!" << endl;
    else
        cout << "Error! The file " << fileName << " cannot be opened!" << endl;
}
void read_file(ifstream &file, string arr[], int size) {
    for (int x=0; x<size; x++)
        file >> arr[x];
}
void read_file_lines(ifstream &file, double weightM[][5], double distanceM[][5], int deliveriesM[][5]) {
    int customerIndex, productIndex;
    double weight, distance;
    while (file >> customerIndex >> productIndex >> weight >> distance) {
        weightM[customerIndex][productIndex] += weight;
        distanceM[customerIndex][productIndex] += distance;
        deliveriesM[customerIndex][productIndex]++;
    }
}
void weight_filter(double matrix[][5], bool boolM[][5], int rows, int columns) {
    for (int i=0; i<rows; i++) {
        for (int j=0; j<columns; j++) {
            if (matrix[i][j] > 13000)
                boolM[i][j] = true;
        }
    }
}
void best_customer_list(bool boolM[][5], double distanceM[][5], int list[], int &bestCustomer, int rows, int columns) {
    int bestCostumerBools = 0;
    for (int i=0; i<rows; i++) {
        int bools = 0;
        for (int j=0; j<columns; j++) {
            if (boolM[i][j])
                bools++;
        }
        if (bools > bestCostumerBools) {
            bestCostumerBools = bools;
            bestCustomer = i;
        }
    }
    list[0] = 0;
    for (int index=1; index<columns; index++) {
        int i = index-1;
        while (i>=0 && distanceM[bestCustomer][index]<distanceM[bestCustomer][list[i]]) {
            list[i+1] = list[i];
            i--;
        }
        list[i+1] = index;
    }
}
void product_deliveries(int deliveriesM[][5], int &productDeliveries, int productIndex, int rows) {
    for (int i=0; i<rows; i++)
        productDeliveries += deliveriesM[i][productIndex];
}

int main() {
    ifstream namesFile, productsFile;
    open_file(namesFile, "Nombres.txt");
    open_file(productsFile, "Datos.txt");
    if (!productsFile || !namesFile) {
        namesFile.close();
        productsFile.close();
        return 1;
    }
    int customersArrSize = 8;
    int productsArrSize = 5;
    string customersArr[customersArrSize];
    string productsArr[productsArrSize];
    read_file(namesFile, customersArr, customersArrSize);
    read_file(namesFile, productsArr, productsArrSize);
    double weightM[customersArrSize][5] {}, distanceM[customersArrSize][5] {};
    int deliveriesM[customersArrSize][5] {};
    read_file_lines(productsFile, weightM, distanceM, deliveriesM);
    namesFile.close();
    productsFile.close();
    cout << endl;
    bool boolM[customersArrSize][5] {};
    weight_filter(weightM, boolM, customersArrSize, productsArrSize);
    cout << "Customers and products whose weights are over 13000: " << endl;
    for (int i=0; i<customersArrSize; i++) {
        cout << customersArr[i] << ": ";
        bool first = true;
        for (int j=0; j<productsArrSize; j++) {
            if (boolM[i][j] && first) {
                cout << productsArr[j];
                first = false;
            }
            else if (boolM[i][j])
                cout  << ", " << productsArr[j];
        }
        cout << "." << endl;
    }
    cout << endl;
    int bestCustomer {}, list[productsArrSize] {};
    best_customer_list(boolM, distanceM, list, bestCustomer, customersArrSize, productsArrSize);
    cout << customersArr[bestCustomer] << "'s products list and their total kilometers: " << endl;
    for (int j=0; j<productsArrSize; j++)
        cout << productsArr[list[j]] << ": " << distanceM[bestCustomer][list[j]] << " km." << endl;
    cout << endl;
    int productDeliveries {};
    product_deliveries(deliveriesM, productDeliveries, list[productsArrSize-1], customersArrSize);
    cout << "The " << customersArr[bestCustomer] << "'s product with more kilometers traveled is " <<
     productsArr[list[productsArrSize-1]] << "." << endl;
    cout << productDeliveries << " deliveries have been made with this product among all customers." << endl;
    cout << endl;
    return 0;
}