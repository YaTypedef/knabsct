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

// ������ �������� (ISO ��������� ��� ������)
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

// ������ � ����� SAMPLE_ACCOUNTS
struct TAccount {
    size_t Id; // ������������� �������
    size_t BureauCd; // ��� ����, �� �������� ������� ����
    
    TDate BkiRequestDate; // ����, � ������� ��� ������ ������ � ����
    TDate InfConfirmDate; // ���� ������������� ���������� �� �����

    EContractType ContractType; // ��� ��������
    EContractStatus ContractStatus; // ������ ��������

    TDate OpenDate; // ���� �������� ��������
    TDate FinalPaymentDate; // ���� ���������� ������� (��������)
    TDate FactCloseDate; // ���� �������� ����� (�����������)
    
    float CreditLimit; // ��������� �����. ����� � ������ �� ����� �� ��
    ECurrency Currency; // ������ ��������

    float Outstanding; // ���������� ������������ �������������. ����� � ������ �� ����� �� ��
    float NextPayment; // ������ ���������� �������. ����� � ������ �� ����� �� ��
    float CurrentBalanceAmount; // ����� ����������� �����, ������� ����� ��������� �����, ��������, ���� � ������. ����� � ������ �� ����� �� ��

    int CurrentDelq; // ������� ���������� ���� ���������
    int TtlDelq_5; // ���������� ��������� �� 5 ����
    int TtlDelq_5_29; // ���������� ��������� �� 5 �� 29 ����
    int TtlDelq_30_59; // ���������� ��������� �� 30 �� 59 ����
    int TtlDelq_60_89; // ���������� ��������� �� 60 �� 89 ����
    int TtlDelq_30; // ���������� ��������� �� 30 ����
    int TtlDelq_90_Plus; // ���������� ��������� 90+ ����

    TDate PaymentStringStart; // ���� ������ ������ PaymentString_84M
    
    // ���������� (���������������) ��������. ������ ������������ �� ����� ��������� ����� �� ������� �������� ������ ������ �� ����� � ����, ������ ������ - ��������� �� ���� PMT_STRING_START, ����� ��������������� � ������� �������� ���.
    //  0 � �����, ������ ����������
    //  X � ��� ����������
    //  1 � ������ ��� ���������
    //  A � ��������� �� 1 �� 29 ����
    //  2 � ��������� �� 30 �� 59 ����
    //  3 � ��������� �� 60 �� 89 ����
    //  4 � ��������� �� 90 �� 119 ����
    //  5 � ��������� ����� 120 ����
    //  7 � ���������� ����������������� �������
    //  8 � ��������� �� ������� � �������������� ������
    //  9 � ���������� ����/ �������� �� ���������/ ����������� ������
    std::string PaymentString_84M;

    float DelqBalance; // ������� ������������ �������������. ����� � ������ �� ����� �� ��
    float MaxDelqBalance; // ������������ ����� ������������ �������������. ����� � ������ �� ����� �� ��
    float InterestRate; // ���������� ������ �� �������

    EPaymentFrequency PaymentFrequency; // ������� ��������

    ERelationship Relationship; // ��� ��������� � ��������

public:
    std::vector<float> NumericFields; // ����� �������������� ��� �������� ����

public:
    static const size_t INPUT_FIELDS_COUNT = 28; // ���������� ����� � ������ �� ������� ������

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