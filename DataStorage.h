#ifndef __DATASTORAGE_H__
#define __DATASTORAGE_H__

#include <stdint.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>


#define DATA_GROUP_MAX			500			//一个存储器里最大能存放的数据块个数
#define DATA_NUM_MAX			512			//一个数据块里可以存储的数据个数

#define USE_8BITS_DATA
#define USE_16BITS_DATA
#define USE_32BITS_DATA
#define USE_64BITS_DATA

#define USE_FLOAT_DATA
#define USE_DOUBLE_DATA

//存储数据类型，以单个字节的数据为分类标准，
typedef enum Data_t{
	TYPE_U8 = 0,
	TYPE_U16 = 1,
	TYPE_U32 = 2,
	TYPE_U64 = 3,
	TYPE_I8 = 4,
	TYPE_I16 = 5,
	TYPE_I32 = 6,
	TYPE_I64 = 7,
	TPYE_CHAR = 8,
	TYPE_STR = 9,
	TYPE_FLOAT = 10,
	TYPE_DOUBLE = 11,
	TYPE_MAX,
}DataType;

//用于存储外部存储设备的编号，名字任意定义，ID号即是存储设备在设备列表中的索引号
typedef enum StorageDeviceID_t{
	EEPROM = 0,
	FLASH = 1,
	IRAM = 2,
	DEVICE_MAX,
}StorageDeviceIDType;

//整数类型( 包含字符串 )存储结构体定义
typedef struct DataStorage_t{
	uint32_t dataSize;				//数据大小
	uint32_t dataBase;				//数据被分配的基址
	DataType dataType;				//数据类型
}DataStorageType;


//浮点数类型结构体定义
typedef struct NonIntegerData_t{
	uint64_t integer;		//整数
	uint8_t dot;			//小数点分割位，此位之前为整数，此位之后为小数，在16进制中占得位数
	uint64_t decimal;		//小数
	uint8_t nDecimal;		//小数位数，以十进制计算
	uint64_t storeData;		//以十六进制
}NonIntegerDataType;

//浮点数存储结构体定义
typedef struct FloatDataStorage_t{
	NonIntegerDataType floatingData;		//浮点数
	DataStorageType storage;			//存储方式
}FloatDataStorageType;


typedef uint8_t *(*readFunctionType)(StorageDeviceIDType id,DataStorageType *data);
typedef char (*writeFunctionType)(StorageDeviceIDType id, DataStorageType *data, uint8_t *bytes);
typedef void (*initFunctionType)(void);

//存储设备的基本信息
typedef struct StorageDevice_t{
	uint32_t devBase;											// 存储器基址
	uint32_t devSize;											// 存储容量，以字节计算
	readFunctionType read;										//读函数
	writeFunctionType write;									//写函数
	initFunctionType init;										//初始化函数
#if defined(USE_8BITS_DATA) || defined(USE_16BITS_DATA) || defined(USE_32BITS_DATA) || defined(USE_64BITS_DATA)
	DataStorageType dataList[DATA_GROUP_MAX]; 					//存储整形数据的数据类型列表
#endif
#if (defined(USE_FLOAT_DATA) || defined(USE_DOUBLE_DATA)) && (defined(USE_32BITS_DATA) || defined(USE_64BITS_DATA))
	FloatDataStorageType floatDataList[DATA_GROUP_MAX];			//存储浮点型数据类型列表
#endif 
}StorageDeviceType;

//数据分配器，用以管理分配地址和空间
typedef struct DataAssign_t{
	StorageDeviceType devList[DEVICE_MAX];		//数据分配器存储的
	uint32_t curAddr[DEVICE_MAX];				//下一个可分配地址的首地址
	uint32_t reserve[DEVICE_MAX];				//当前存储器剩余的字节数量
}DataAssignerType;

/*
* 函数名：dataAssignerInit
* 参数：StorageDeviceIDType dev_id, 设备ID
* 参数：StorageDeviceType *dev, 设备结构体
* 返回： 无
*/
void dataAssignerInit(StorageDeviceIDType devID, StorageDeviceType *dev);
/*
* 函数名：storageDeviceInit
* 参数：StorageDeviceIDType dev_id, 设备ID
* 参数：uint32_t base, 设备基址
* 参数：uint32_t size, 设备容量大小，以字节计
* 参数：readFunctionType read, 写函数指针
* 参数：writeFunctionType write 读函数指针
* 参数： initFunctionType init 初始化函数指针
* 返回： StorageDeviceType * 返回存储设备结构体首地址
*/
StorageDeviceType *storageDeviceInit(StorageDeviceIDType dev_id, uint32_t base, uint32_t size,
										 readFunctionType read, writeFunctionType write, initFunctionType init);
/*
* 函数名：writeDataToAddress
* 参数：StorageDeviceIDType devID, 设备ID
* 参数：DataStorageType *data, 存储的数据类型描述
* 参数：uint8_t *bytes, 数据指针，以字节计
* 返回： char 写数据到特定器件的特定地址的状态		0 表示成功 非0表示失败
*/
char writeDataToAddress(StorageDeviceIDType devID, DataStorageType *data, uint8_t *bytes);
/*
* 函数名：readDataFromAddress
* 参数：StorageDeviceIDType devID, 设备ID
* 参数：DataStorageType *data, 存储的数据类型描述
* 参数：uint8_t *bytes, 数据指针，以字节计
* 返回： uint8_t * 读特定器件的特定地址获得的数据，以字节计
*/
uint8_t *readDataFromAddress(StorageDeviceIDType devID, DataStorageType *data);

uint32_t * u32ToAddress(uint32_t addr);

#ifdef USE_8BITS_DATA
/*
* 函数名：u8ToAddress
* 参数：uint8_t addr，地址值
* 返回： uint8_t * 转换后的地址指针
*/
	uint8_t *u8ToAddress(uint8_t addr);

void bytesTo8BitsData(uint8_t *inData, DataStorageType *outData);
/*
* 函数名：i8DataToBytes
* 参数：int8_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void i8DataToBytes(int8_t *inData, uint8_t *outData, uint32_t size);
/*
* 函数名：u8DataToBytes
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void u8DataToBytes(uint8_t *inData, uint8_t *outData, uint32_t size);

#endif // USE_8BITS_DATA

#ifdef USE_16BITS_DATA
/*
* 函数名：bytesTo16BitsData
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：DataStorageType outData，转换后的数据类型
* 返回： 无
*/
	void bytesTo16BitsData(uint8_t *inData, DataStorageType *outData);
/*
* 函数名：i16DataToBytes
* 参数：int16_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void i16DataToBytes(int16_t *inData, uint8_t *outData, uint32_t size);
/*
* 函数名：u16DataToBytes
* 参数：uint16_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void u16DataToBytes(uint16_t *inData, uint8_t *outData, uint32_t size);
/*
* 函数名：u16ToAddress
* 参数：uint16_t addr，地址值
* 返回： uint16_t * 转换后的地址指针
*/
	uint16_t *u16ToAddress(uint16_t addr);
#endif // USE_16BITS_DATA

#ifdef USE_32BITS_DATA
/*
* 函数名：bytesTo32BitsData
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：DataStorageType outData，转换后的数据类型
* 返回： 无
*/
	void bytesTo32BitsData(uint8_t *inData, DataStorageType *outData);
/*
* 函数名：i32DataToBytes
* 参数：int32_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void i32DataToBytes(int32_t *inData, uint8_t *outData, uint32_t size);
/*
* 函数名：u32DataToBytes
* 参数：uint32_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void u32DataToBytes(uint32_t *inData, uint8_t *outData, uint32_t size);
	/*
* 函数名：u32ToAddress
* 参数：uint32_t addr，地址值
* 返回： uint32_t * 转换后的地址指针
*/
	uint32_t *u32ToAddress(uint32_t addr);
#endif // USE_32BITS_DATA

#ifdef USE_64BITS_DATA
/*
* 函数名：bytesTo64BitsData
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：DataStorageType outData，转换后的数据类型
* 返回： 无
*/
	void bytesTo64BitsData(uint8_t *inData, DataStorageType *outData);
/*
* 函数名：i64DataToBytes
* 参数：int64_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void i64DataToBytes(int64_t *inData, uint8_t *outData, uint32_t size);
/*
* 函数名：u64DataToBytes
* 参数：uint64_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
	void u64DataToBytes(uint64_t *inData, uint8_t *outData, uint32_t size);
	/*
* 函数名：u64ToAddress
* 参数：uint64_t addr，地址值
* 返回： uint64_t * 转换后的地址指针
*/
	uint64_t *u64ToAddress(uint64_t addr);
#endif // USE_64BITS_DATA

#if defined(USE_FLOAT_DATA) || defined(USE_DOUBLE_DATA)
/*
* 函数名：floatTo64BitsData
* 参数：float data，需要转换的数据
* 返回： NonIntegerDataType * 转换成浮点型结构体
*/
	NonIntegerDataType *floatTo64BitsData(float data);
/*
* 函数名：doubleTo64BitsData
* 参数：double data，需要转换的数据
* 返回： NonIntegerDataType * 转换成浮点型结构体
*/
	NonIntegerDataType *doubleTo64BitsData(double data);
/*
* 函数名：bytesToFloat
* 参数：NonIntegerDataType* floatData, 存储在StorageDeviceType的floatDataList数组里的成员
* 参数：uint64_t data，需要转换的数据
* 返回： float 浮点数值
*/
	float bytesToFloat(NonIntegerDataType *floatData, uint64_t data);
/*
* 函数名：bytesToDouble
* 参数：NonIntegerDataType* floatData, 存储在StorageDeviceType的floatDataList数组里的成员
* 参数：uint64_t data，需要转换的数据
* 返回： double 浮点数值
*/
	double bytesToDouble(NonIntegerDataType *floatData, uint64_t data);

#endif //defined(USE_FLOAT_DATA) || defined(USE_DOUBLE_DATA)

#endif //__DATASTORAGE_H__

