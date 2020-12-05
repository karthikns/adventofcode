// 1_1.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <algorithm>

uint64_t input[] = {
        1883,
        1543,
        1801,
        1731,
        1070,
        1631,
        1490,
        1179,
        1098,
        1582,
        1717,
        1830,
        1408,
        1524,
        889,
        985,
        2005,
        1540,
        1085,
        1607,
        1518,
        1993,
        1496,
        1537,
        1514,
        1719,
        1218,
        1420,
        1027,
        1339,
        1430,
        989,
        1613,
        1970,
        1227,
        1082,
        1079,
        1068,
        1674,
        1186,
        1744,
        1297,
        1467,
        1647,
        1141,
        1825,
        1759,
        1395,
        1596,
        1405,
        1844,
        1461,
        1762,
        1122,
        1173,
        1838,
        1983,
        1632,
        1995,
        1245,
        1454,
        1120,
        1671,
        1526,
        1572,
        1929,
        1642,
        1864,
        1351,
        1155,
        1885,
        1226,
        1810,
        1252,
        1061,
        1882,
        2002,
        1627,
        1128,
        1575,
        1750,
        1046,
        1767,
        1270,
        1037,
        1198,
        1942,
        1074,
        1820,
        1301,
        1382,
        1687,
        1824,
        1996,
        1704,
        1051,
        1546,
        1431,
        1102,
        1041,
        1547,
        1202,
        1875,
        1800,
        1433,
        1901,
        1165,
        1151,
        1785,
        1903,
        1278,
        1185,
        1940,
        1935,
        1479,
        1495,
        719,
        1683,
        1972,
        1483,
        1589,
        1636,
        1055,
        1317,
        1530,
        1990,
        1099,
        1697,
        1286,
        1089,
        1136,
        1383,
        1802,
        1618,
        1050,
        1980,
        1279,
        1777,
        1635,
        1721,
        1660,
        1569,
        1554,
        1432,
        1695,
        1551,
        1601,
        1263,
        1866,
        1998,
        1466,
        1205,
        1445,
        1578,
        1267,
        1873,
        1610,
        1900,
        1192,
        1827,
        1305,
        1528,
        1140,
        1440,
        1269,
        1748,
        1187,
        52,
        1149,
        1603,
        1033,
        1650,
        1045,
        1345,
        1710,
        1955,
        1891,
        1392,
        1870,
        1357,
        1197,
        1087,
        1690,
        1090,
        622,
        1590,
        1304,
        1533,
        1971,
        1959,
        1842,
        1172,
        1653,
        1093,
        1299,
        1203,
        1119,
        1193,
        1223,
        1291,
};

using namespace std;

using MyVec = vector<uint64_t>;
using MyVecIter = MyVec::iterator;
uint64_t FindTwo(MyVecIter begin, MyVecIter end, uint64_t amount)
{
    for (MyVecIter first = begin; first != end; ++first)
    {
        auto second = find(first, end, amount - *first);
        if (second != end)
        {
            return *first * *second;
        }
    }

    return 0;
}
// 0 1 2 3 4 5
// 0     Find2([1 2 3 4 5], 2020 - val[0])
// 1     Find2([2 3 4 5], 2020 - val[1])
// 2     Find2([3 4 5], 2020 - val[2])
// 3     Find2([4 5], 2020 - val[3])
uint64_t FindThree(MyVec& inputVec)
{
    MyVecIter last = end(inputVec);
    --last;
    --last;

    for (auto it = begin(inputVec); it != last; ++it)
    {
        uint64_t val = FindTwo(it + 1, end(inputVec), 2020 - *it);
        if (val != 0)
            return val * *it;
    }

    return 0;
}
/*
input [1,2,3]
0x100 - 1
0x108 - 2
0x110 - 3

input -> 0x100
sizeof(input) -> 24
sizeof(input[0]) -> 8

0x100 + 3

0x100 + 3*sizeof(uint64_t)
0x100 + 3*8
0x100 + 24
0x100 + 0x018
0x118

0x100, 0x118
*/

int main()
{
    vector<uint64_t> inputVec(input, input + sizeof(input) / sizeof(input[0]));

    //sort(inputVec.begin(), inputVec.end());

    //for (uint64_t val : inputVec)
    //{
    //    cout << val << endl;
    //}

    for (uint64_t val : inputVec)
    {
        //cout << val << endl;

        // 1000
        // 2020 - 1000 = 1020
        // (1000, 1020)

        auto it = find(begin(inputVec), end(inputVec), 2020 - val);
        if (it != end(inputVec))
        {
            //cout << val << endl << *it << endl;

            cout << val * *it << endl;
            break;
        }
    }

    //1-1
    cout << FindTwo(begin(inputVec), end(inputVec), 2020) << endl;

    //1-2
    cout << FindThree(inputVec) << endl;

    std::cout << "Hello World!\n";
}
