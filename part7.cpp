#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>
#include <cmath>
#include <iomanip>

using namespace std;

class BaseTransaction {
protected:
    string maGD;
    double soTien;
    string danhMuc;
    string moTa;
    string ngayTao;

public:
    BaseTransaction(string id, double tien, string dm,
                    string desc = "", string dt = "2026-09-14")
        : maGD(id), soTien(tien), danhMuc(dm),
          moTa(desc), ngayTao(dt) {}

    virtual ~BaseTransaction() {}

    virtual double laySoTienThucTe() const = 0;
    virtual string layLoaiGiaoDich() const = 0;

    string getMaGD() const {
        return maGD;
    }

    double getSoTien() const {
        return soTien;
    }

    string getDanhMuc() const {
        return danhMuc;
    }

    string getMoTa() const {
        return moTa;
    }

    string getNgayTao() const {
        return ngayTao;
    }
};

class IncomeTransaction : public BaseTransaction {
public:
    IncomeTransaction(string id, double tien, string dm,
                      string desc = "", string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return abs(soTien);
    }

    string layLoaiGiaoDich() const override {
        return "Thu Nhap";
    }
};

class ExpenseTransaction : public BaseTransaction {
public:
    ExpenseTransaction(string id, double tien, string dm,
                       string desc = "", string dt = "2026-09-14")
        : BaseTransaction(id, tien, dm, desc, dt) {}

    double laySoTienThucTe() const override {
        return -abs(soTien);
    }

    string layLoaiGiaoDich() const override {
        return "Chi Phi";
    }
};

class RecurringExpenseTransaction : public ExpenseTransaction {
private:
    string chuKy;

public:
    RecurringExpenseTransaction(string id, double tien, string dm,
                                string ck = "Hang thang",
                                string desc = "",
                                string dt = "2026-09-14")
        : ExpenseTransaction(id, tien, dm, desc, dt),
          chuKy(ck) {}

    string getChuKy() const {
        return chuKy;
    }

    string layLoaiGiaoDich() const override {
        return "Chi Phi Dinh Ky";
    }
};

class ExpenseChart {
private:
    vector<shared_ptr<BaseTransaction>> danhSachGiaoDich;

public:

    void themGiaoDich(shared_ptr<BaseTransaction> giaoDich) {
        if (giaoDich != nullptr) {
            danhSachGiaoDich.push_back(giaoDich);
        }
    }

    map<string, double> tinhChiTieuTheoThang() const {

        map<string, double> chiTieuTheoThang;

        for (const auto& giaoDich : danhSachGiaoDich) {

            if (giaoDich->laySoTienThucTe() < 0) {

                string ngay = giaoDich->getNgayTao();
              
                if (ngay.length() >= 7) {

                    string thang = ngay.substr(0, 7);

                    double soTienChi =
                        abs(giaoDich->laySoTienThucTe());

                    chiTieuTheoThang[thang] += soTienChi;
                }
            }
        }

        return chiTieuTheoThang;
    }


    void hienThiBieuDo() const {

        map<string, double> chiTieuTheoThang =
            tinhChiTieuTheoThang();

        cout << "\n==============================================\n";
        cout << "       BIEU DO CHI TIEU HANG THANG\n";
        cout << "==============================================\n";

        if (chiTieuTheoThang.empty()) {
            cout << "Chua co du lieu chi tieu.\n";
            return;
        }

        double chiTieuLonNhat = 0;

        for (const auto& item : chiTieuTheoThang) {
            if (item.second > chiTieuLonNhat) {
                chiTieuLonNhat = item.second;
            }
        }


        for (const auto& item : chiTieuTheoThang) {

            string thang = item.first;
            double soTien = item.second;

            int soCot = 0;

            if (chiTieuLonNhat > 0) {
                soCot = (int)(soTien / chiTieuLonNhat * 40);
            }

            if (soTien > 0 && soCot == 0) {
                soCot = 1;
            }

            cout << thang << " | ";

            for (int i = 0; i < soCot; i++) {
                cout << "█";
            }

            cout << " " << fixed << setprecision(0)
                 << soTien << " VNĐ\n";
        }

        cout << "==============================================\n";
        cout << "Chu thich: Moi cot dai toi da 40 ky tu.\n";
    }
};

int main() {

    ExpenseChart bieuDo;
  
    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD01",
            500000,
            "An uong",
            "An toi",
            "2026-07-05"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD02",
            1000000,
            "Tien nha",
            "Tien tro",
            "2026-07-15"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD03",
            800000,
            "An uong",
            "An trua",
            "2026-08-03"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD04",
            1500000,
            "Tien nha",
            "Tien tro",
            "2026-08-15"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD05",
            1200000,
            "An uong",
            "An toi",
            "2026-09-02"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD06",
            1800000,
            "Tien nha",
            "Tien tro",
            "2026-09-10"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<RecurringExpenseTransaction>(
            "GD07",
            500000,
            "Giai tri",
            "Hang thang",
            "Goi dich vu",
            "2026-09-20"
        )
    );


    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD08",
            700000,
            "An uong",
            "An uong",
            "2026-10-03"
        )
    );

    bieuDo.themGiaoDich(
        make_shared<ExpenseTransaction>(
            "GD09",
            900000,
            "Di lai",
            "Xang xe",
            "2026-10-12"
        )
    );

    bieuDo.hienThiBieuDo();


    return 0;
}

