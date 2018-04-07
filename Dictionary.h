#ifndef _Preferences_
#define _Preferences_

#include "EEPROM.h"
#include "WString.h"

typedef const char* tag_t;

class Dictionary {
  public:

    bool write(tag_t tag, size_t size, const uint8_t* data);  // return true on success
    size_t read(tag_t tag, size_t size, uint8_t* data);
    size_t size(tag_t tag);

    bool write(String& tag, String& data);
    size_t read(String& tag, String& data);

    void reset();  // danger: this erases all the prefs

    uint8_t getVersion() { return version; }
    size_t used();  // returns total amount of space used
    size_t size() { return totalPrefsSize; }  // returns total size of prefs space
    size_t remaining() { return size() - used(); }
    static constexpr size_t MAX_TAG_SIZE = 255;

    const uint8_t* data() { return prefsData; }  // for debugging only

  protected:
    size_t totalPrefsSize = 0;
    uint8_t* prefsData = nullptr;
    virtual void save() {};

    uint8_t version;
    static constexpr tag_t versionTag = "vers";
    static constexpr tag_t endTag = "end";

    size_t writeTag(size_t offset, tag_t tag, size_t size, const uint8_t* data);
    size_t findTag(tag_t tag);
    size_t tagDataSize(size_t offset);
    void deleteTag(size_t start);
    size_t tagSize(size_t offset);
};

#if TEENSY
#define EEPROM_SIZE (E2END+1)
#else
#define EEPROM_SIZE 4096
#endif

class EEPROMDictionary : public Dictionary {
  public:
    EEPROMDictionary() { totalPrefsSize = EEPROM_SIZE; prefsData = EEPromData; }
    void load(uint8_t initVersion = 1);  // 1 - default version

  protected:
    virtual void save();

    uint8_t EEPromData[EEPROM_SIZE];  // this keeps a full copy of the flash in RAM, which is wasteful, but avoids complications of using FlexRAM directly
};

#endif
