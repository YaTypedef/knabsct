#pragma once

#include <string>
#include <unordered_map>

struct TDate {
    size_t Day;
    size_t Month;
    size_t Year;

    bool IsUnknown;

public:
    TDate();
    TDate(const std::string& str);
};

enum EContractType {
    CT_UNKNOWN = -1,
    CT_CAR = 1,
    CT_LEASING = 4,
    CT_HYPOTHEC = 6,
    CT_CREDIT_CARD = 7,
    CT_CONSUMER_CREDIT = 9,
    CT_BUSINESS_CREDIT = 10,
    CT_CIRCULATING_ASSESTS_CREDIT = 11,
    CT_EQUIPMENT_CREDIT = 12,
    CT_REALTY_CREDIT = 13,
    CT_STOCK_CREDIT = 14,
    CT_OTHER = 99
};

enum EContractStatus {
    CS_UNKNOWN = -1,
    CS_ACTIVE = 0,
    CS_PAYED = 12,
    CS_CLOSED = 13,
    CS_SENT_TO_OTHER_BANK = 14,
    CS_CONTROVERSY = 21,
    CS_OVERDUE = 52,
    CS_PROBLEMS = 61
};

// Валюта договора (ISO буквенный код валюты)
enum ECurrency {
    C_UNKNOWN = -1,
    C_RUB = 0,
    C_USD = 1,
    C_OTHER =2
};

enum EPaymentFrequency {
    PF_UNKNOWN = -1,
    PF_DAY = 1,
    PF_TWO_WEEKS = 2,
    PF_MONTH = 3,
    PF_TWO_MONTHS = 4,
    PF_THREE_MONTHS = 5,
    PF_FOUR_MONTHS = 6,
    PF_SIX_MONTHS = 7,
    PF_YEAR = 8,
    PF_OTHER = 9
};

enum ERelationship {
    R_UNKNOWN = -1,
    R_NATURAL_PERSON = 1,
    R_AUTHORIZED = 2,
    R_MUTUAL = 4,
    R_GUARANTEE = 5,
    R_LEGAL_ENTITY = 9,
};

// Строка в файле SAMPLE_ACCOUNTS
struct TAccount {
    size_t Id; // Идентификатор клиента
    size_t BureauCd; // Код бюро, из которого получен счет
    
    TDate BkiRequestDate; // Дата, в которую был сделан запрос в бюро
    TDate InfConfirmDate; // Дата подтверждения информации по счету

    EContractType ContractType; // Тип договора
    EContractStatus ContractStatus; // Статус договора

    TDate OpenDate; // Дата открытия договора
    TDate FinalPaymentDate; // Дата финального платежа (плановая)
    TDate FactCloseDate; // Дата закрытия счета (фактическая)
    
    float CreditLimit; // Кредитный лимит. Сумма в рублях по курсу ЦБ РФ
    ECurrency Currency; // Валюта договора

    float Outstanding; // Оставшаяся непогашенная задолженность. Сумма в рублях по курсу ЦБ РФ
    float NextPayment; // Размер следующего платежа. Сумма в рублях по курсу ЦБ РФ
    float CurrentBalanceAmount; // Общая выплаченная сумма, включая сумму основного долга, проценты, пени и штрафы. Сумма в рублях по курсу ЦБ РФ

    int CurrentDelq; // Текущее количество дней просрочки
    int TtlDelq_5; // Количество просрочек до 5 дней
    int TtlDelq_5_29; // Количество просрочек от 5 до 29 дней
    int TtlDelq_30_59; // Количество просрочек от 30 до 59 дней
    int TtlDelq_60_89; // Количество просрочек от 60 до 89 дней
    int TtlDelq_30; // Количество просрочек до 30 дней
    int TtlDelq_90_Plus; // Количество просрочек 90+ дней

    TDate PaymentStringStart; // Дата начала строки PaymentString_84M
    
    // Дисциплина (своевременность) платежей. Строка составляется из кодов состояний счета на моменты передачи банком данных по счету в бюро, первый символ - состояние на дату PMT_STRING_START, далее последовательно в порядке убывания дат.
    //  0 – Новый, оценка невозможна
    //  X – Нет информации
    //  1 – Оплата без просрочек
    //  A – Просрочка от 1 до 29 дней
    //  2 – Просрочка от 30 до 59 дней
    //  3 – Просрочка от 60 до 89 дней
    //  4 – Просрочка от 90 до 119 дней
    //  5 – Просрочка более 120 дней
    //  7 – Регулярные консолидированные платежи
    //  8 – Погашение по кредиту с использованием залога
    //  9 – Безнадёжный долг/ передано на взыскание/ пропущенный платеж
    std::string PaymentString_84M;

    float DelqBalance; // Текущая просроченная задолженность. Сумма в рублях по курсу ЦБ РФ
    float MaxDelqBalance; // Максимальный объем просроченной задолженности. Сумма в рублях по курсу ЦБ РФ
    float InterestRate; // Процентная ставка по кредиту

    EPaymentFrequency PaymentFrequency; // Частота платежей

    ERelationship Relationship; // Тип отношения к договору

public:
    std::vector<float> NumericFields; // Здесь продублированы все числовые поля

public:
    static const size_t INPUT_FIELDS_COUNT = 28; // Количество полей в строке во входных данных

public:
    TAccount();
    TAccount(const std::string& str);

private:
    EContractType ContractTypeFromString(const std::string& str) const;
    EContractStatus ContractStatusFromString(const std::string& str) const;
    ECurrency CurrencyFromString(const std::string& str) const;
    EPaymentFrequency PaymentFrequencyFromString(const std::string& str) const;
    ERelationship RelationshipFromString(const std::string& str) const;
};

struct TData {
    // id -> vector of accounts
    typedef std::unordered_map<size_t, std::vector<TAccount>> THashData;
    THashData Data;
    
    // it -> target
    std::unordered_map<size_t, float> LearnTargets;
    std::unordered_map<size_t, float> TestTargets;

public:
    void LoadFromCSVFiles(const std::string& accountsFile, const std::string& customersFile);
};