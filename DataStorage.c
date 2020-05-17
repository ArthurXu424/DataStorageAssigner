#include "DataStorage.h"

static DataAssignerType dataAssigner; 			//数据分配器
static StorageDeviceType deviceList[DEVICE_MAX];

#ifdef USE_8BITS_DATA
	static uint8_t u8Data[DATA_NUM_MAX];
	static int8_t i8Data[DATA_NUM_MAX];
#endif // USE_8BITS_DATA

#ifdef USE_16BITS_DATA
	static uint16_t u16Data[DATA_NUM_MAX];
	static int16_t i16Data[DATA_NUM_MAX];
#endif // USE_16BITS_DATA

#ifdef USE_32BITS_DATA
	static uint32_t u32Data[DATA_NUM_MAX];
	static int32_t i32Data[DATA_NUM_MAX];
#endif // USE_32BITS_DATA

#ifdef USE_64BITS_DATA
	static uint64_t u64Data[DATA_NUM_MAX];
	static int64_t i64Data[DATA_NUM_MAX];
#endif // USE_64BITS_DATA

#if defined(USE_FLOAT_DATA) || defined(USE_DOUBLE_DATA)
	NonIntegerDataType floatData;
	FloatDataStorageType floatingStorage;
#endif
		/*
* 函数名：dataAssignerInit
* 参数：StorageDeviceIDType dev_id, 设备ID
* 参数：StorageDeviceType *dev, 设备结构体
* 返回： 无
*/
void dataAssignerInit(StorageDeviceIDType devID, StorageDeviceType *dev)
{
	dataAssigner.devList[devID] = *dev;
	dataAssigner.curAddr[devID] = 0;
	dataAssigner.reserve[devID] = dev->devSize;
}

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
									 readFunctionType read, writeFunctionType write, initFunctionType init)
{
	deviceList[dev_id].devBase = base;
	deviceList[dev_id].devSize = size;
#if defined(USE_FLOAT_DATA) || defined(USE_DOUBLE_DATA)
	//deviceList[dev_id].floatDataList;
#endif
#if defined(USE_8BITS_DATA) || defined(USE_16BITS_DATA) || defined(USE_32BITS_DATA) || defined(USE_64BITS_DATA)
	//deviceList[dev_id].dataList;
#endif
	deviceList[dev_id].read = read;
	deviceList[dev_id].write = write;
	deviceList[dev_id].init = init;
	deviceList[dev_id].init();
	return &deviceList[dev_id];
}

/*
* 函数名：writeDataToAddress
* 参数：StorageDeviceIDType devID, 设备ID
* 参数：DataStorageType *data, 存储的数据类型描述
* 参数：uint8_t *bytes, 数据指针，以字节计
* 返回： char 写数据到特定器件的特定地址的状态		0 表示成功 非0表示失败
*/
char writeDataToAddress(StorageDeviceIDType devID, DataStorageType *data, uint8_t *bytes)
{
	DataStorageType dsType;
	dsType.dataSize = data->dataSize;
	dsType.dataBase = dataAssigner.curAddr[devID];
	dsType.dataType = data->dataType;
	dataAssigner.curAddr[devID] += data->dataSize;
	dataAssigner.reserve[devID] -= data->dataSize;
	return dataAssigner.devList[devID].write(&dsType,bytes);
}

/*
* 函数名：readDataFromAddress
* 参数：StorageDeviceIDType devID, 设备ID
* 参数：DataStorageType *data, 存储的数据类型描述
* 参数：uint8_t *bytes, 数据指针，以字节计
* 返回： uint8_t * 读特定器件的特定地址获得的数据，以字节计
*/
uint8_t *readDataFromAddress(StorageDeviceIDType devID, DataStorageType *data)
{
	return dataAssigner.devList[devID].read(devID,data);
}


#ifdef USE_8BITS_DATA
/*
* 函数名：u8ToAddress
* 参数：uint8_t addr，地址值
* 返回： uint8_t * 转换后的地址指针
*/
uint8_t *u8ToAddress(uint8_t addr)
{
	return (uint8_t *)addr;
}
/*
* 函数名：u8DataToBytes
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void u8DataToBytes(uint8_t* inData,uint8_t* outData, uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size>>24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_U8;
	outData++;
	for(i=0;i<size;i++)
	{
		*outData = *inData;
		outData++;
		inData++;
	}
}
#endif // USE_8BITS_DATA

#ifdef USE_16BITS_DATA

/*
* 函数名：u16ToAddress
* 参数：uint16_t addr，地址值
* 返回： uint16_t * 转换后的地址指针
*/
uint16_t *u16ToAddress(uint16_t addr)
{
	return (uint16_t *)addr;
}
/*
* 函数名：u16DataToBytes
* 参数：uint16_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void u16DataToBytes(uint16_t *inData, uint8_t *outData, uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size >> 24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_U16;
	outData++;
	for (i = 0; i < size*2; i++)
	{
		*outData = (uint8_t)((*inData)>>8);
		outData++;
		*outData = (uint8_t)(*inData);
		outData++;
		inData++;
	}
}
#endif //USE_16BITS_DATA

#ifdef USE_32BITS_DATA
/*
* 函数名：u32ToAddress
* 参数：uint32_t addr，地址值
* 返回： uint32_t * 转换后的地址指针
*/
uint32_t *u32ToAddress(uint32_t addr)
{
	return (uint32_t *)addr;
}
/*
* 函数名：u32DataToBytes
* 参数：uint32_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void u32DataToBytes(uint32_t *inData, uint8_t *outData, uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size >> 24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_U32;
	outData++;
	for (i = 0; i < size * 4; i++)
	{
		*outData = (uint8_t)((*inData) >> 24);
		outData++;
		*outData = (uint8_t)((*inData) >> 16);
		outData++;
		*outData = (uint8_t)((*inData) >> 8);
		outData++;
		*outData = (uint8_t)(*inData);
		outData++;
		inData++;
	}
}
#endif // USE_32BITS_DATA

#ifdef USE_64BITS_DATA
/*
* 函数名：u64ToAddress
* 参数：uint64_t addr，地址值
* 返回： uint64_t * 转换后的地址指针
*/
uint64_t *u64ToAddress(uint64_t addr)
{
	return (uint64_t *)addr;
}
/*
* 函数名：u64DataToBytes
* 参数：uint64_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void u64DataToBytes(uint64_t *inData, uint8_t *outData, uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size >> 24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_I32;
	outData++;
	for (i = 0; i < size * 8; i++)
	{
		*outData = (uint8_t)((*inData) >> 56);
		outData++;
		*outData = (uint8_t)((*inData) >> 48);
		outData++;
		*outData = (uint8_t)((*inData) >> 40);
		outData++;
		*outData = (uint8_t)((*inData) >> 32);
		outData++;
		*outData = (uint8_t)((*inData) >> 24);
		outData++;
		*outData = (uint8_t)((*inData) >> 16);
		outData++;
		*outData = (uint8_t)((*inData) >> 8);
		outData++;
		*outData = (uint8_t)(*inData);
		outData++;
		inData++;
	}
}
#endif //USE_64BITS_DATA

#ifdef USE_8BITS_DATA
/*
* 函数名：i8DataToBytes
* 参数：int8_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void i8DataToBytes(int8_t *inData, uint8_t *outData, uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size >> 24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_I8;
	outData++;
	for (i = 0; i < size; i++)
	{
		*outData = (uint8_t)(*inData);
		outData++;
		inData++;
	}
}
#endif //USE_8BITS_DATA

#ifdef USE_16BITS_DATA
/*
* 函数名：i16DataToBytes
* 参数：int16_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void i16DataToBytes(int16_t *inData, uint8_t *outData, uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size >> 24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_I16;
	outData++;
	for (i = 0; i < size * 2; i++)
	{
		*outData = (uint8_t)((*inData) >> 8);
		outData++;
		*outData = (uint8_t)(*inData);
		outData++;
		inData++;
	}
}
#endif // USE_16BITS_DATA

#ifdef USE_32BITS_DATA
/*
* 函数名：i32DataToBytes
* 参数：int32_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void i32DataToBytes(int32_t *inData, uint8_t *outData, uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size >> 24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_I32;
	outData++;
	for (i = 0; i < size * 4; i++)
	{
		*outData = (uint8_t)((*inData) >> 24);
		outData++;
		*outData = (uint8_t)((*inData) >> 16);
		outData++;
		*outData = (uint8_t)((*inData) >> 8);
		outData++;
		*outData = (uint8_t)(*inData);
		outData++;
		inData++;
	}
}
#endif // USE_32BITS_DATA

#ifdef USE_64BITS_DATA
/*
* 函数名：i64DataToBytes
* 参数：int64_t* inData，需要转换的数据地址指针
* 参数：uint8_t* outData，转换后的数据地址指针
* 参数：uint32_t size，需要转换的数据个数
* 返回： 无
*/
void i64DataToBytes(int64_t* inData, uint8_t* outData,uint32_t size)
{
	int i = 0;
	*outData = (uint8_t)size >> 24;
	outData++;
	*outData = (uint8_t)size >> 16;
	outData++;
	*outData = (uint8_t)size >> 8;
	outData++;
	*outData = (uint8_t)size;
	outData++;
	*outData = TYPE_I32;
	outData++;
	for (i = 0; i < size * 8; i++)
	{
		*outData = (uint8_t)((*inData) >> 56);
		outData++;
		*outData = (uint8_t)((*inData) >> 48);
		outData++;
		*outData = (uint8_t)((*inData) >> 40);
		outData++;
		*outData = (uint8_t)((*inData) >> 32);
		outData++;
		*outData = (uint8_t)((*inData) >> 24);
		outData++;
		*outData = (uint8_t)((*inData) >> 16);
		outData++;
		*outData = (uint8_t)((*inData) >> 8);
		outData++;
		*outData = (uint8_t)(*inData);
		outData++;
		inData++;
	}
}
#endif // USE_64BITS_DATA

#ifdef USE_8BITS_DATA
/*
* 函数名：bytesTo8BitsData
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：DataStorageType outData，转换后的数据类型
* 返回： 无
*/
void bytesTo8BitsData(uint8_t *inData, DataStorageType *outData)
{
	uint32_t len = 0;
	int i = 0;
	len = *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	outData->dataSize = len;
	inData++;
	outData->dataType = *inData;
	inData++;
	if(outData->dataType == TYPE_I8)
		memset(i8Data, 0, DATA_NUM_MAX);
	else
		memset(u8Data, 0, DATA_NUM_MAX);
	for (i = 0; i < len; i++)
	{
		switch (outData->dataType)
		{
			case TYPE_U8:
			case TYPE_STR:
				u8Data[i] = *inData;
				break;
			case TYPE_I8:
				i8Data[i] = (int8_t)*inData;
				break;
			default:
				break;
		}
		inData++;
	}
}
#endif //USE_8BITS_DATA

#ifdef USE_16BITS_DATA
/*
* 函数名：bytesTo16BitsData
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：DataStorageType outData，转换后的数据类型
* 返回： 无
*/
void bytesTo16BitsData(uint8_t *inData, DataStorageType *outData)
{
	uint32_t len = 0;
	int i = 0;
	len = *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	outData->dataSize = len;
	inData++;
	outData->dataType = *inData;
	inData++;
	if (outData->dataType == TYPE_I16)
		memset(i16Data, 0, DATA_NUM_MAX);
	else
		memset(u16Data, 0, DATA_NUM_MAX);
	for (i = 0; i < len/2; i++)
	{
		uint16_t dat = *inData;
		inData++;
		dat = (dat << 8) + *inData;
		switch (outData->dataType)
		{
			case TYPE_I16:
				i16Data[i] = (int16_t)dat;
				break;
			case TYPE_U16:
				u16Data[i] = dat;
				break;
			default:
				break;
		}
		inData++;
	}
}
#endif // USE_16BITS_DATA

#ifdef USE_32BITS_DATA
/*
* 函数名：bytesTo32BitsData
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：DataStorageType outData，转换后的数据类型
* 返回： 无
*/
void bytesTo32BitsData(uint8_t *inData, DataStorageType *outData)
{
	uint32_t len = 0;
	int i = 0;
	len = *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	outData->dataSize = len;
	inData++;
	outData->dataType = *inData;
	inData++;
	if (outData->dataType == TYPE_I32)
		memset(i32Data, 0, DATA_NUM_MAX);
	else
		memset(u32Data, 0, DATA_NUM_MAX);
	for (i = 0; i < len / 2; i++)
	{
		uint32_t dat = *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		switch (outData->dataType)
		{
			case TYPE_U32:
				u32Data[i] = dat;
				break;
			case TYPE_I32:
				i32Data[i] = (int32_t)dat;
				break;
			default:
				break;
		}
		inData++;
	}
}
#endif // USE_32BITS_DATA

#ifdef USE_64BITS_DATA
/*
* 函数名：bytesTo64BitsData
* 参数：uint8_t* inData，需要转换的数据地址指针
* 参数：DataStorageType outData，转换后的数据类型
* 返回： 无
*/
void bytesTo64BitsData(uint8_t *inData, DataStorageType *outData)
{
	uint32_t len = 0;
	int i = 0;
	len = *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	inData++;
	len = (len << 8) + *inData;
	outData->dataSize = len;
	inData++;
	outData->dataType = *inData;
	inData++;
	if (outData->dataType == TYPE_I64)
		memset(i64Data, 0, DATA_NUM_MAX);
	else
		memset(u64Data, 0, DATA_NUM_MAX);
	for (i = 0; i < len / 2; i++)
	{
		uint64_t dat = *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		inData++;
		dat = (dat << 8) + *inData;
		switch (outData->dataType)
		{
			case TYPE_U64:
				u64Data[i] = dat;
				break;
			case TYPE_I64:
				i64Data[i] = (int64_t)dat;
				break;
			default:
				break;
		}
		inData++;
	}
}
#endif // USE_64BITS_DATA

#if defined(USE_FLOAT_DATA) || defined(USE_DOUBLE_DATA)
/*
* 函数名：floatTo64BitsData
* 参数：float data，需要转换的数据
* 返回： NonIntegerDataType * 转换成浮点型结构体
*/
NonIntegerDataType *floatTo64BitsData(float data) //浮点数以64位存储
{
	uint8_t i = 0;
	uint64_t integerData = (uint64_t) data;			//整数部分
	uint64_t decimalData = 0;
	float dec = (data - integerData);	//小数部分
	floatData.integer = integerData;
	for(i=0;i<64;i++)
	{
		if (integerData & 0x8000000000000000)
		{
			floatData.dot = 64 - i;
			break;
		}
		integerData = integerData << 1;
	}
	for(i = 0;i<20;i++)		//最大支持小数点后20位 
	{
		float decTemp = 0.0;
		dec = dec*10;
		decTemp = dec - (uint64_t)dec;	//小数部分
		if(decTemp == 0.0)
		{
			decimalData = (uint64_t)dec;			//将小数部分转换成整数存储
			floatData.nDecimal = i+1;				//小数的位数，以十进制计算
			break;
		}
	}
	floatData.decimal = decimalData;
	floatData.storeData = ((floatData.integer << floatData.dot) + (floatData.decimal));
	return &floatData;
}
/*
* 函数名：doubleTo64BitsData
* 参数：double data，需要转换的数据
* 返回： NonIntegerDataType * 转换成浮点型结构体
*/
NonIntegerDataType *doubleTo64BitsData(double data) //浮点数以64位存储
{
	uint8_t i = 0;
	uint64_t integerData = (uint64_t)data; //整数部分
	uint64_t decimalData = 0;
	double dec = (data - integerData); //小数部分
	floatData.integer = integerData;
	for (i = 0; i < 64; i++)
	{
		if (integerData & 0x8000000000000000)
		{
			floatData.dot = 64 - i;
			break;
		}
		integerData = integerData << 1;
	}
	for (i = 0; i < 20; i++) //最大支持小数点后20位
	{
		double decTemp = 0.0;
		dec = dec * 10;
		decTemp = dec - (uint64_t)dec; //小数部分
		if (decTemp == 0.0)
		{
			decimalData = (uint64_t)dec; //将小数部分转换成整数存储
			floatData.nDecimal = i + 1;	 //小数的位数，以十进制计算
			break;
		}
	}
	floatData.decimal = decimalData;
	floatData.storeData = ((floatData.integer << floatData.dot) + (floatData.decimal));
	return &floatData;
}
/*
* 函数名：bytesToFloat
* 参数：NonIntegerDataType* floatData, 存储在StorageDeviceType的floatDataList数组里的成员
* 参数：uint64_t data，需要转换的数据
* 返回： float 浮点数值
*/
float bytesToFloat(NonIntegerDataType* floatData, uint64_t data)
{
	uint64_t intData = 0;
	uint64_t decData = 0;
	uint8_t i = 0;
	float fdata = 0.0;
	float dec = 0.0;
	floatData->storeData = data;
	intData = floatData->storeData >> floatData->dot;
	decData = (floatData->storeData & ((0x0000000000000001<<floatData->dot) - 1));
	fdata = (float)intData;			//整数部分
	for(i=0;i<fdata->nDecimal;i++)
	{
		uint8_t b = decData % 10;
		dec = dec*0.1 + b*0.1;
		decData = decData / 10;
	}
	fdata +=  dec;					//小数部分
	return fdata;
}
/*
* 函数名：bytesToDouble
* 参数：NonIntegerDataType* floatData, 存储在StorageDeviceType的floatDataList数组里的成员
* 参数：uint64_t data，需要转换的数据
* 返回： double 浮点数值
*/
double bytesToDouble(NonIntegerDataType *floatData, uint64_t data)
{
	uint64_t intData = 0;
	uint64_t decData = 0;
	uint8_t i = 0;
	double fdata = 0.0;
	double dec = 0.0;
	floatData->storeData = data;
	intData = floatData->storeData >> floatData->dot;
	decData = (floatData->storeData & ((0x0000000000000001 << floatData->dot) - 1));
	fdata = (float)intData; //整数部分
	for (i = 0; i < fdata->nDecimal; i++)
	{
		uint8_t b = decData % 10;
		dec = dec * 0.1 + b * 0.1;
		decData = decData / 10;
	}
	fdata += dec; //小数部分
	return fdata;
}

#endif // defined(USE_FLOAT_DATA) || defined(USE_DOUBLE_DATA)
