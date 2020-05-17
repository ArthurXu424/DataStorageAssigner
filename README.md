# DataStorageAssigner
 
1,What DataStorageAssigner is?
     DataStorageAssigner is a library to manually manage the storage of your own data in multi-devices,such as EEPROM, FLASH, RAM etc.
2,Preparation
    you need to offer your device access interface ( init, write and read ) fisrt by invoke storageDeviceInit
3,Usage
    a, assign device ID in StorageDeviceID_t of file DataStorage.h
    b, init your device via call storageDeviceInit
    c, access your data, read or write via calling dataAssigner
    d, for u8 u16 u32 u64 i8 i16 i32 i64, you can directly access
    e, for float and double , it will be converted to u64 first via calling corresponding fuction 
    f, for string, stored as u8, analyze when read.
    
4,Contact Author
    you can contact author via mail:arthurxu0424@126.com
    

