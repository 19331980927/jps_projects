/*-------------------------------- Arctic Core ------------------------------
 * Dio.h - AUTOSAR DIO Driver Interface
 */

#ifndef DIO_H_
#define DIO_H_

#include "Std_Types.h"

typedef uint32 Dio_ChannelType;
typedef uint32 Dio_PortLevelType;

typedef struct {
    uint32 port;
} Dio_ChannelGroupType;

typedef uint32 Dio_LevelType;

void Dio_WriteChannel(Dio_ChannelType ChannelId, Dio_LevelType Level);
Dio_LevelType Dio_ReadChannel(Dio_ChannelType ChannelId);
void Dio_ReadChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr,
                          Dio_PortLevelType *PortLevelPtr);
void Dio_WriteChannelGroup(const Dio_ChannelGroupType *ChannelGroupIdPtr,
                           Dio_PortLevelType Level);
void Dio_GetVersionInfo(Std_VersionInfoType *versioninfo);

#endif /* DIO_H_ */
