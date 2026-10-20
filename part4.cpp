#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <memory>

struct ScheduledRule {
    std::shared_ptr<RecurringExpenseTransaction> templateTransaction; 
    Account* targetAccount;                                             
    std::string lastProcessedMonth;                                     
};
class TransactionScheduler {
private:
    std::vector<ScheduledRule> scheduledList;
    int autoIdCounter;

public:
    TransactionScheduler() : autoIdCounter(100) {}

  
    void registerRecurringTransaction(const std::shared_ptr<RecurringExpenseTransaction>& rExpense, Account* acc) {
        if (!rExpense || !acc) {
            std::cout << "[Loi Scheduler] Du lieu dang ky khong hop le (Null Pointer)!\n";
            return;
        }

        ScheduledRule rule;
        rule.templateTransaction = rExpense;
        rule.targetAccount = acc;
        rule.lastProcessedMonth = "";

        scheduledList.push_back(rule);
        std::cout << "[Scheduler] Da dang ky lich: '" << rExpense->getMoTa() 
                  << "' cho tai khoan " << acc->getTenTK() << "\n";
    }

 
    void processMonthlyTransactions(const std::string& yearMonth, std::map<std::string, Budget>& budgets) {
        std::cout << "\n======================================================\n";
        std::cout << " MO PHONG KICH HOAT CHI PHI DINH KY THANG: " << yearMonth << "\n";
        std::cout << "======================================================\n";

        if (scheduledList.empty()) {
            std::cout << "(Chua co lich trinh chi phi dinh ky nao trong he thong)\n";
            return;
        }

        for (auto& rule : scheduledList) {
          
            if (rule.lastProcessedMonth == yearMonth) {
                std::cout << "- Khoan '" << rule.templateTransaction->getMoTa() 
                          << "' da duoc xu ly trong thang " << yearMonth << " roi.\n";
                continue;
            }

            std::string newId = "AUTO" + std::to_string(++autoIdCounter);
            std::string actualDate = yearMonth + "-01"; 

            auto actualTransaction = std::make_shared<ExpenseTransaction>(
                newId,
                rule.templateTransaction->getSoTien(),
                rule.templateTransaction->getDanhMuc(),
                "[Dinh ky] " + rule.templateTransaction->getMoTa(),
                actualDate
            );

            std::cout << "* Kich hoat khoan chi dinh ky: " << rule.templateTransaction->getMoTa() << "...\n";
            
            bool success = rule.targetAccount->thucHienGiaoDich(actualTransaction);

            if (success) {
                rule.lastProcessedMonth = yearMonth; 
              
                std::string category = rule.templateTransaction->getDanhMuc();
                if (budgets.find(category) != budgets.end()) {
                    budgets[category].addExpense(rule.templateTransaction->getSoTien());
                    std::cout << "  -> [Cap nhat ngan sach] Danh muc '" << category 
                              << "' | Trang thai: " << budgets[category].getStatus() << "\n";
                }
            }
        }
    }

    void processMonthlyTransactions(const std::string& yearMonth) {
        std::map<std::string, Budget> emptyBudgets;
        processMonthlyTransactions(yearMonth, emptyBudgets);
    }

    size_t getScheduledCount() const {
        return scheduledList.size();
    }
};

int main() {
    Account mb("TK02", "MB Bank", "Ngan hang", 5000000.0);
    Account vmm("TK03", "Vi MoMo", "Vi dien tu", 800000.0);

    std::cout << "--- TRANG THAI TAI KHOAN BAN DAU ---\n";
    mb.inThongTin();
    vmm.inThongTin();
    std::cout << "\n";

    auto tienTro = std::make_shared<RecurringExpenseTransaction>(
        "REC01", 2500000, "Tien nha", "Hang thang", 5, "Tro co dinh hang thang"
    );
    
    auto spotify = std::make_shared<RecurringExpenseTransaction>(
        "REC02", 59000, "Giai tri", "Hang thang", 1, "Goi Spotify Sinh Vien"
    );

    TransactionScheduler scheduler;
    scheduler.registerRecurringTransaction(tienTro, &mb);
    scheduler.registerRecurringTransaction(spotify, &vmm);

    scheduler.processMonthlyTransactions("2026-09");

    std::cout << "\n--- SO DU SAU KHI TU DONG TRU CHI PHI THANG 09/2026 ---\n";
    mb.inThongTin();
    vmm.inThongTin();

    scheduler.processMonthlyTransactions("2026-10");

    std::cout << "\n--- SO DU SAU KHI TU DONG TRU CHI PHI THANG 10/2026 ---\n";
    mb.inThongTin();
    vmm.inThongTin();

    return 0;
}
