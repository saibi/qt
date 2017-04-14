#ifndef PARSER_H
#define PARSER_H

#include <QList>
#include <QStringList>

// Interface records
#define _ANS_		"ANS"
#define _CANDO_		"CANDO"
#define _CRC_		"CRC"
#define _CRIBFMT_	"CRIBFMT"
#define _D_		"D"
#define _DEF_		"DEF"
#define _DEV_		"DEV"
#define _DO_		"DO"
#define _DRLFMT_	"DRLFMT"
#define _DSDEV_		"DSDEV"
#define _ENDDEF_	"ENDDEF"
#define _HID_		"HID"
#define _INFO_		"INFO"
#define _IP_		"IP"
#define _JOB_		"JOB"
#define _LIB_		"LIB"
#define _MESG_		"MESG"
#define _MID_		"MID"
#define _MNAME_		"MNAME"
#define _MODEL_		"MODEL"
#define _MSL_		"MSL"
#define _MVER_		"MVER"
#define _OMAV_		"OMAV"
#define _OPERID_	"OPERID"
#define _PORT_		"PORT"
#define _REM_		"REM"
#define _REQ_		"REQ"
#define _RMT_		"RMT"
#define _SDFMODE_	"SDFMODE"
#define _SN_		"SN"
#define _STATUS_	"STATUS"
#define _STHKFMT_	"STHKFMT"
#define _SURFMT_	"SURFMT"
#define _TIMEOUT_	"TIMEOUT"
#define _TRCFMT_	"TRCFMT"
#define _VEN_		"VEN"
#define _XSTATUS_	"XSTATUS"
#define _ZFMT_		"ZFMT"

// Request types (literal data)
#define _INI_	"INI" // Initialization
#define _TRC_	"TRC" // Frame trace upload
#define _PTG_	"PTG" // Pattern generator download
#define _EDG_	"EDG" // Edger download
#define _SBK_	"SBK" // Surface blocker download
#define _FBK_	"FBK" // Finish blocker download
#define _AGN_	"AGN" // Laminator download
#define _GEN_	"GEN" // Surface generator download
#define _UPL_	"UPL" // Generic upload
#define _DNL_	"DNL" // Generic download
#define _MNT_	"MNT" // Maintenance upload
#define _ERR_	"ERR" // Error answer response
#define _INF_	"INF" // Information upload
#define _COA_	"COA" // Surface coater download
#define _FSG_	"FSG" // Direct surface generator download
#define _LMD_	"LMD" // Lens measuring device download
#define _INS_	"INS" // Inspection device upload
#define _LAP_	"LAP" // Lap feeder download
#define _DRL_	"DRL" // Drill download
#define _CON_	"CON" // Network connection request
#define _LDS_	"LDS" // Lens Design System download
#define _LMS_	"LMS" // Host upload from LDS
#define _FSP_	"FSP" // Direct surface polisher download
#define _POL_	"POL" // Surfacer download
#define _ENG_	"ENG" // Lens engraver download
#define _SDF_	"SDF" // Surface Definition File Identifier
#define _TIM_	"TIM" // Time Synchronization Request
#define _LDI_	"LDI" // LDS initialization file identifier
#define _LMI_	"LMI" // LMS initialization file identifier
#define _BRS_	"BRS" // Blank Selection Request (to LDS from LMS)
#define _BAS_	"BAS" // Blank Selection Response (to LMS from LDS)
#define _FIL_   "FIL" // File

class Packet;
class Record;
class RecordSet;

class Parser
{
private:
    Parser();

public:
    //
    static int crc16ccitt(char* pData, int size);

    // Raw data -> Record
    static bool parseRecord(const char* pData, int size, Record& record);
    static bool parseRecord(const QString& data, Record& record);

    // Packet <-> RecordSet
    static bool parsePacketToRecordSet(const Packet& packet, RecordSet& recordSet);
    static bool makePacketFromRecordSet(Packet& packet, const RecordSet& recordSet, bool crcSepFlag = true);

    static bool makeFormatByteArrayFromRecordSet(QByteArray& outputData, const RecordSet& recordSet);

    static bool verifyPacketCrc(Packet& packet);

    static void decodeEscape(const char* pData, int size, QByteArray& outputData);

    // Record ?;?;?;... -> String list
    static void splitRecordValues(const Record& record, QStringList& values);

    // ASCII absolute format - TRCFMT = 1;*;*;*;*
    static void splitRecordValues_AsciiAbsFormat(const Record& record, QList<int>& values, bool clearFlag = false);
    // Binary absolute format - TRCFMT = 2;*;*;*;*
    static void splitRecordValues_BinAbsFormat(const Record& record, QList<int>& values, bool clearFlag = false);
    // Binary differential format - TRCFMT = 3;*;*;*;*
    static void splitRecordValues_BinDiffFormat(const Record& record, QList<int>& values, bool clearFlag = false);
    // Packed binary format - TRCFMT = 4;*;*;*;*
    static void splitRecordValues_PackedBinFormat(const Record& record, QList<int>& values, bool clearFlag = false);
    // Huvitz internal format 5 - TRCFMT = 5;*;*;*;*
    static void splitRecordValues_Huvitz5Format(const Record& record, QList<int>& values, bool clearFlag = false);
    // Huvitz internal format 6 - TRCFMT = 6;*;*;*;*
    static void splitRecordValues_Huvitz6Format(const Record& record, QList<int>& values, bool clearFlag = false);

    static void addRecordsToSet_AsciiAbsFormat(const QList<int>& values, RecordSet& recordSet, const QString& key);
    static void addRecordsToSet_BinAbsFormat(const QList<int>& values, RecordSet& recordSet, const QString& key);
    static void addRecordsToSet_BinDiffFormat(const QList<int>& values, RecordSet& recordSet, const QString& key);
    static void addRecordsToSet_PackedBinFormat(const QList<int>& values, RecordSet& recordSet, const QString& key);
    static void addRecordsToSet_Huvitz5Format(const QList<int>& values, RecordSet& recordSet, const QString& key);
    static void addRecordsToSet_Huvitz6Format(const QList<int>& values, RecordSet& recordSet, const QString& key);
};

#endif // PARSER_H
