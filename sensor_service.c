/**
  ******************************************************************************
  * @file    sensor_service.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    04-July-2014
  * @brief   Add a sample service using a vendor specific profile.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2014 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
#include "sensor_service.h"

/** @addtogroup X-CUBE-BLE1_Applications
 *  @{
 */

/** @addtogroup SensorDemo
 *  @{
 */
 
/** @defgroup SENSOR_SERVICE
 * @{
 */

/** @defgroup SENSOR_SERVICE_Private_Variables
 * @{
 */
/* Private variables ---------------------------------------------------------*/
volatile int connected = FALSE;
volatile uint8_t set_connectable = 1;
volatile uint16_t connection_handle = 0;
volatile uint8_t notification_enabled = FALSE;
volatile AxesRaw_t axes_data = {0, 0, 0};
uint16_t sampleServHandle, TXCharHandle, RXCharHandle;


////////////////////////////////////////////////////////////////
// Our Code
////////////////////////////////////////////////////////////////
uint16_t temperatureServHandle, temperatureCharHandle;
uint16_t accelerometerServHandle, pitchCharHandle, rollCharHandle;
uint16_t doubletapServHandle, doubletapCharHandle;
uint16_t ledcntrlServHandle, ledcntrlCharHandle;

int16_t AccPitch;
int16_t AccRoll;
int16_t Temperature;
uint8_t DoubleTap;
uint8_t LEDControl;

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////

/**
 * @}
 */

/** @defgroup SENSOR_SERVICE_Private_Macros
 * @{
 */
/* Private macros ------------------------------------------------------------*/
#define COPY_UUID_128(uuid_struct, uuid_15, uuid_14, uuid_13, uuid_12, uuid_11, uuid_10, uuid_9, uuid_8, uuid_7, uuid_6, uuid_5, uuid_4, uuid_3, uuid_2, uuid_1, uuid_0) \
do {\
    uuid_struct[0] = uuid_0; uuid_struct[1] = uuid_1; uuid_struct[2] = uuid_2; uuid_struct[3] = uuid_3; \
        uuid_struct[4] = uuid_4; uuid_struct[5] = uuid_5; uuid_struct[6] = uuid_6; uuid_struct[7] = uuid_7; \
            uuid_struct[8] = uuid_8; uuid_struct[9] = uuid_9; uuid_struct[10] = uuid_10; uuid_struct[11] = uuid_11; \
                uuid_struct[12] = uuid_12; uuid_struct[13] = uuid_13; uuid_struct[14] = uuid_14; uuid_struct[15] = uuid_15; \
}while(0)


////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Our Code 
////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define COPY_TEMPERATURE_SERVICE_UUID(uuid_struct)	COPY_UUID_128(uuid_struct, 0x13,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_TEMPERATURE_CHAR_UUID(uuid_struct)			COPY_UUID_128(uuid_struct, 0x13,0x35,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
	
#define COPY_ACCELEROMETER_SERVICE_UUID(uuid_struct)	COPY_UUID_128(uuid_struct, 0x12,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_PITCH_CHAR_UUID(uuid_struct)							COPY_UUID_128(uuid_struct, 0x12,0x35,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_ROLL_CHAR_UUID(uuid_struct)							COPY_UUID_128(uuid_struct, 0x12,0x34,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
	
#define COPY_DOUBLETAP_SERVICE_UUID(uuid_struct)	COPY_UUID_128(uuid_struct, 0x11,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_DOUBLETAP_CHAR_UUID(uuid_struct)			COPY_UUID_128(uuid_struct, 0x11,0x35,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
	
#define COPY_LEDCNTRL_SERVICE_UUID(uuid_struct)		COPY_UUID_128(uuid_struct, 0x10,0x36,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
#define COPY_LEDCNTRL_CHAR_UUID(uuid_struct)			COPY_UUID_128(uuid_struct, 0x10,0x35,0x6e,0x80, 0xcf,0x3a, 0x11,0xe1, 0x9a,0xb4, 0x00,0x02,0xa5,0xd5,0xc5,0x1b)
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

/* Store Value into a buffer in Little Endian Format */
#define STORE_LE_16(buf, val)    ( ((buf)[0] =  (uint8_t) (val)    ) , \
                                   ((buf)[1] =  (uint8_t) (val>>8) ) )
/**
 * @}
 */
///////////////////////////////////////////////////////////////////////////////////////////
// Our Code
///////////////////////////////////////////////////////////////////////////////////////////

/**
	* @brief Adds bluetooth service for the temperature sensor and one characteristic which contains the filtered sensor data
	* @param none
	* @retval Determines whether operation was successful or not
	*/
tBleStatus Add_Temperature_Service(void)
{
	tBleStatus ret;
	
	uint8_t uuid[16];
	//uint16_t uuid16;
	//haractFormat charFormat;
	//uint16_t descHandle;
	
	Temperature = 0;
	
	// Add the temperature service
	COPY_TEMPERATURE_SERVICE_UUID(uuid);
	ret = aci_gatt_add_serv(UUID_TYPE_128, uuid, PRIMARY_SERVICE, 7, &temperatureServHandle);
	if(ret != BLE_STATUS_SUCCESS) goto fail;
	
	// Add the temperature characteristic
	COPY_TEMPERATURE_CHAR_UUID(uuid);
	ret = aci_gatt_add_char(temperatureServHandle, UUID_TYPE_128, uuid, 2, CHAR_PROP_READ | CHAR_PROP_NOTIFY, 
													ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &temperatureCharHandle);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error while adding Temperature characteristic\n");
		goto fail;
	}
	
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error while adding Temperature characteristic description\n");
		goto fail;
	}
	
	printf("Service Temperature added. Handle 0x%04X, Temperature Charac handle: 0x%04X\n", temperatureServHandle, temperatureCharHandle);
	return BLE_STATUS_SUCCESS;
	
	fail:
	printf("Error while adding Temperature Service\n");
	return BLE_STATUS_ERROR;
}

/**
	* @brief Adds bluetooth service for the accelerometer sensor and two characteristic which contain the pitch and roll angle
	* @param none
	* @retval Determines whether operation was successful or not
	*/
tBleStatus Add_Accelerometer_Service(void)
{
	tBleStatus ret;
	
	AccPitch = 0;
	AccRoll = 0;
	
	uint8_t uuid[16];
	
	// Add the Accelerometer service
	COPY_ACCELEROMETER_SERVICE_UUID(uuid);
	ret = aci_gatt_add_serv(UUID_TYPE_128, uuid, PRIMARY_SERVICE, 7, &accelerometerServHandle);
	if(ret != BLE_STATUS_SUCCESS) goto fail;
	
	// Add the Pitch characteristic
	COPY_PITCH_CHAR_UUID(uuid);
	ret = aci_gatt_add_char(accelerometerServHandle, UUID_TYPE_128, uuid, 2, CHAR_PROP_READ | CHAR_PROP_NOTIFY, 
													ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &pitchCharHandle);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error while adding Pitch characteristic\n");
		goto fail;
	}
	
	// Add the Roll characteristic
	COPY_ROLL_CHAR_UUID(uuid);
	ret = aci_gatt_add_char(accelerometerServHandle, UUID_TYPE_128, uuid, 2, CHAR_PROP_READ | CHAR_PROP_NOTIFY, 
													ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &rollCharHandle);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error while adding the Roll characteristic\n");
		goto fail;
	}
	
	printf("Service Accelerometer added. Handle 0x%04X, Pitch Charac handle: 0x%04X, Roll Charac handle 0x%04X\n", accelerometerServHandle, pitchCharHandle, rollCharHandle);
	return BLE_STATUS_SUCCESS;
	
	fail:
	printf("Error adding Accelerometer Service\n");
	return BLE_STATUS_ERROR;
}

/**
	* @brief Adds bluetooth service for the doubletap function and one characteristic which will inform the the android device if a double tap has occured.
	* @param none
	* @retval Determines whether operation was successful or not
	*/
tBleStatus Add_Doubletap_Service(void)
{
	tBleStatus ret;
	
	DoubleTap = 0;
	
	uint8_t uuid[16];
	
	// Add the Doubletap service
	COPY_DOUBLETAP_SERVICE_UUID(uuid);
	ret = aci_gatt_add_serv(UUID_TYPE_128, uuid, PRIMARY_SERVICE, 7, &doubletapServHandle);
	if(ret != BLE_STATUS_SUCCESS) goto fail;
	
	// Add the Doubletap characteristic
	COPY_DOUBLETAP_CHAR_UUID(uuid);
	ret = aci_gatt_add_char(doubletapServHandle, UUID_TYPE_128, uuid, 1, CHAR_PROP_NOTIFY, 
													ATTR_PERMISSION_NONE, 0, 16, 0, &doubletapCharHandle);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error while addind Doubletap characteristic\n");
		goto fail;
	}
	
	printf("Service Doubletap added. Handle 0x%04X, Doubletap Charac handle: 0x%04X\n", doubletapServHandle, doubletapCharHandle);
	return BLE_STATUS_SUCCESS;
	
	fail:
	printf("Error adding Doubletap Service\n");
	return BLE_STATUS_ERROR;
}

/**
	* @brief Adds bluetooth service for the LED control and one characteristic which contains the state of the LEDs
	* @param none
	* @retval Determines whether operation was successful or not
	*/
tBleStatus Add_LEDcntrl_Service(void)
{
	tBleStatus ret;
	
	LEDControl = 0;
	
	uint8_t uuid[16];
	
	// Add the LEDcntrl service
	COPY_LEDCNTRL_SERVICE_UUID(uuid);
	ret = aci_gatt_add_serv(UUID_TYPE_128, uuid, PRIMARY_SERVICE, 7, &ledcntrlServHandle);
	if(ret != BLE_STATUS_SUCCESS) goto fail;
	
	// Add the LEDcntrl characteristic
	COPY_LEDCNTRL_CHAR_UUID(uuid);
	ret = aci_gatt_add_char(ledcntrlServHandle, UUID_TYPE_128, uuid, 1, CHAR_PROP_READ | CHAR_PROP_NOTIFY, 
													ATTR_PERMISSION_NONE, GATT_NOTIFY_READ_REQ_AND_WAIT_FOR_APPL_RESP, 16, 0, &ledcntrlCharHandle);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error while adding LEDcntrl characteristic\n");
		goto fail;
	}
	
	printf("Service LEDcntrl added. Handle 0x%04X, LEDcntrl Charac handle: 0x%04X\n", ledcntrlServHandle, ledcntrlCharHandle);
	return BLE_STATUS_SUCCESS;
	
	fail:
	printf("Error adding LEDcntrl Service\n");
	return BLE_STATUS_ERROR;
}

/**
	* @brief Updates the temperature characteristic
	* @param Temperature in tenths of a degree
	* @retval Determines whether operation was successful or not
*/
tBleStatus Update_Temperature(int16_t temp)
{
	tBleStatus ret;
	uint8_t buffer[2];
	
	STORE_LE_16(buffer,temp);
	
	//Update Temperature characteristic value
	ret = aci_gatt_update_char_value(temperatureServHandle, temperatureCharHandle, 0, 2, buffer);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error updating Temperature characteristic\n");
	}
	else
	{
		printf("Temperature characteristic successfully updated\n");
	}
	return ret;
}

/**
	* @brief Updates the Pitch characteristic 
	* @param Pitch value in 100th of a degree
	* @retval Determines whether operation was successful or not
*/
tBleStatus Update_Pitch(int16_t pitch)
{
	tBleStatus ret;
	
	uint8_t buffer[2];
	
	STORE_LE_16(buffer,pitch);
	
	// Update Pitch Characteristic
	ret = aci_gatt_update_char_value(accelerometerServHandle, pitchCharHandle, 0, 2, buffer);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error updating Pitch characteristic\n");
	}
	else
	{
		printf("Pitch characteristic successfully updated\n");
	}
	return ret;
}

/**
	* @brief Updates the Pitch characteristic 
	* @param Pitch value in 100th of a degree
	* @retval Determines whether operation was successful or not
*/
tBleStatus Update_Roll(int16_t roll)
{
	tBleStatus ret;
	
	uint8_t buffer[2];
	
	STORE_LE_16(buffer,roll);
	// Update Roll Characteristic
	ret = aci_gatt_update_char_value(accelerometerServHandle, rollCharHandle, 0, 2, buffer);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error updating Roll characteristic\n");
	}
	else
	{
		printf("Roll characteristic successfully updated\n");
	}
	
	return ret;
}

/**
	* @brief Updates the doubletap charateristic
	* @param Value determines whether doubletap has occured
	* @retval Determines whether operation was successful or not
*/
tBleStatus Update_Doubletap(uint8_t dt)
{
	tBleStatus ret;
	
	// Update Doubletap Characteristic
	ret = aci_gatt_update_char_value(doubletapServHandle, doubletapCharHandle, 0, 1, &dt);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error updating Doubletap characteristic\n");
	}
	else
	{
		printf("Doubletap characteristic successfully updated\n");
	}
	
	return ret;
}
/**
	* @brief Updates the LEDcntrl characteristic
	* @param Control value which determines LED flashing pattern
	* @retval Determines whether operation was successful or not
*/
tBleStatus Update_LEDcntrl(uint8_t ledcntrl)
{
	tBleStatus ret;
	
	// Update LEDcntrl characteristic
	ret = aci_gatt_update_char_value(ledcntrlServHandle, ledcntrlCharHandle, 0, 1, &ledcntrl);
	if(ret != BLE_STATUS_SUCCESS)
	{
		printf("Error updating LEDcntrl characteristic\n");
	}
	else
	{
		printf("LEDcntrl characteristic successfully updated\n");
	}
	
	return ret;
}
///////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////


/**
 * @brief  Puts the device in connectable mode.
 *         If you want to specify a UUID list in the advertising data, those data can
 *         be specified as a parameter in aci_gap_set_discoverable().
 *         For manufacture data, aci_gap_update_adv_data must be called.
 * @param  None 
 * @retval None
 */
/* Ex.:
 *
 *  tBleStatus ret;    
 *  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'B','l','u','e','N','R','G'};    
 *  const uint8_t serviceUUIDList[] = {AD_TYPE_16_BIT_SERV_UUID,0x34,0x12};    
 *  const uint8_t manuf_data[] = {4, AD_TYPE_MANUFACTURER_SPECIFIC_DATA, 0x05, 0x02, 0x01};
 *  
 *  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
 *                                 8, local_name, 3, serviceUUIDList, 0, 0);    
 *  ret = aci_gap_update_adv_data(5, manuf_data);
 *
 */
void setConnectable(void)
{  
  tBleStatus ret;
  
  const char local_name[] = {AD_TYPE_COMPLETE_LOCAL_NAME,'G','r','o','u','p','0','6'};
  
  /* disable scan response */
  hci_le_set_scan_resp_data(0,NULL);
  PRINTF("General Discoverable Mode.\n");
  
  ret = aci_gap_set_discoverable(ADV_IND, 0, 0, PUBLIC_ADDR, NO_WHITE_LIST_USE,
                                 sizeof(local_name), local_name, 0, NULL, 0, 0);
  if (ret != BLE_STATUS_SUCCESS) {
    PRINTF("Error while setting discoverable mode (%d)\n", ret);    
  }  
}

/**
 * @brief  This function is called when there is a LE Connection Complete event.
 * @param  uint8_t Address of peer device
 * @param  uint16_t Connection handle
 * @retval None
 */
void GAP_ConnectionComplete_CB(uint8_t addr[6], uint16_t handle)
{  
  connected = TRUE;
  connection_handle = handle;
  
  PRINTF("Connected to device:");
  for(int i = 5; i > 0; i--){
    PRINTF("%02X-", addr[i]);
  }
  PRINTF("%02X\n", addr[0]);
}

/**
 * @brief  This function is called when the peer device gets disconnected.
 * @param  None 
 * @retval None
 */
void GAP_DisconnectionComplete_CB(void)
{
  connected = FALSE;
  PRINTF("Disconnected\n");
  /* Make the device connectable again. */
  set_connectable = TRUE;
  notification_enabled = FALSE;
}

/**
 * @brief  Read request callback.
 * @param  uint16_t Handle of the attribute
 * @retval None
 */
void Read_Request_CB(uint16_t handle)
{  
  if(handle == pitchCharHandle + 1){
		Update_Pitch(AccPitch);
  }
	else if(handle == rollCharHandle + 1){
		Update_Roll(AccRoll);
  }
	else if(handle == temperatureCharHandle + 1){
		Update_Temperature(Temperature);
	}
	else if(handle == ledcntrlCharHandle + 1){
		Update_LEDcntrl(LEDControl);
	}
	else if(handle == doubletapCharHandle + 1){
		Update_Doubletap(DoubleTap);
	}
//  else if(handle == tempCharHandle + 1){
//    int16_t data;
//    data = 210 + ((uint64_t)rand()*15)/RAND_MAX; //sensor emulation        
//    Acc_Update((AxesRaw_t*)&axes_data); //FIXME: to overcome issue on Android App
//                                        // If the user button is not pressed within
//                                        // a short time after the connection,
//                                        // a pop-up reports a "No valid characteristics found" error.
//    Temp_Update(data);
//  }
  
  //EXIT:
  if(connection_handle != 0)
    aci_gatt_allow_read(connection_handle);
}

/**
 * @brief  Callback processing the ACI events.
 * @note   Inside this function each event must be identified and correctly
 *         parsed.
 * @param  void* Pointer to the ACI packet
 * @retval None
 */
void HCI_Event_CB(void *pckt)
{
  hci_uart_pckt *hci_pckt = pckt;
  /* obtain event packet */
  hci_event_pckt *event_pckt = (hci_event_pckt*)hci_pckt->data;
  
  if(hci_pckt->type != HCI_EVENT_PKT)
    return;
  
  switch(event_pckt->evt){
    
  case EVT_DISCONN_COMPLETE:
    {
      GAP_DisconnectionComplete_CB();
    }
    break;
    
  case EVT_LE_META_EVENT:
    {
      evt_le_meta_event *evt = (void *)event_pckt->data;
      
      switch(evt->subevent){
      case EVT_LE_CONN_COMPLETE:
        {
          evt_le_connection_complete *cc = (void *)evt->data;
          GAP_ConnectionComplete_CB(cc->peer_bdaddr, cc->handle);
        }
        break;
      }
    }
    break;
    
  case EVT_VENDOR:
    {
      evt_blue_aci *blue_evt = (void*)event_pckt->data;
      switch(blue_evt->ecode){

#if NEW_SERVICES
      case EVT_BLUE_GATT_ATTRIBUTE_MODIFIED:         
        {
          /* this callback is invoked when a GATT attribute is modified
          extract callback data and pass to suitable handler function */
          if (bnrg_expansion_board == IDB05A1) {
            evt_gatt_attr_modified_IDB05A1 *evt = (evt_gatt_attr_modified_IDB05A1*)blue_evt->data;
            Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data); 
          }
          else {
            evt_gatt_attr_modified_IDB04A1 *evt = (evt_gatt_attr_modified_IDB04A1*)blue_evt->data;
            Attribute_Modified_CB(evt->attr_handle, evt->data_length, evt->att_data); 
          }                       
        }
        break; 
#endif

      case EVT_BLUE_GATT_READ_PERMIT_REQ:
        {
          evt_gatt_read_permit_req *pr = (void*)blue_evt->data;                    
          Read_Request_CB(pr->attr_handle);                    
        }
        break;
      }
    }
    break;
  }    
}

#if NEW_SERVICES
/**
 * @brief  Add a time service using a vendor specific profile
 * @param  None
 * @retval Status
 */
tBleStatus Add_Time_Service(void)
{
  tBleStatus ret;
  uint8_t uuid[16];
  
  /* copy "Timer service UUID" defined above to 'uuid' local variable */
  COPY_TIME_SERVICE_UUID(uuid);
  
  /* 
   * now add "Time service" to GATT server, service handle is returned
   * via 'timeServHandle' parameter of aci_gatt_add_serv() API. 
   * Please refer to 'BlueNRG Application Command Interface.pdf' for detailed
   * API description 
  */
  ret = aci_gatt_add_serv(UUID_TYPE_128,  uuid, PRIMARY_SERVICE, 7,
                          &timeServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  /* 
   * now add "Seconds characteristic" to Time service, characteristic handle 
   * is returned via 'secondsCharHandle' parameter of aci_gatt_add_char() API.
   * This characteristic is read only, as specified by CHAR_PROP_READ parameter.
   * Please refer to 'BlueNRG Application Command Interface.pdf' for detailed
   * API description 
  */  
  COPY_TIME_UUID(uuid);
  ret =  aci_gatt_add_char(timeServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_READ, ATTR_PERMISSION_NONE, 0,
                           16, 0, &secondsCharHandle);
  
  if (ret != BLE_STATUS_SUCCESS) goto fail;  
  
  COPY_MINUTE_UUID(uuid);  
  /* 
   * Add "Minutes characteristic" to "Time service". 
   * This characteristic is readable as well as notifiable only, as specified 
   * by CHAR_PROP_NOTIFY|CHAR_PROP_READ parameter below.   
   */
  ret = aci_gatt_add_char(timeServHandle, UUID_TYPE_128, uuid, 4,
                          CHAR_PROP_NOTIFY|CHAR_PROP_READ, ATTR_PERMISSION_NONE, 0,
                          16, 1, &minuteCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;
  
  PRINTF("Service TIME added. Handle 0x%04X, TIME Charac handle: 0x%04X\n",timeServHandle, secondsCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
  /* return BLE_STATUS_ERROR if we reach this tag */
fail:
  PRINTF("Error while adding Time service.\n");
  return BLE_STATUS_ERROR ;    
}

/**
 * @brief  Update seconds characteristic value of Time service.
 * @param  None
 * @retval Status
 */
tBleStatus Seconds_Update(void)
{  
  uint32_t val;
  tBleStatus ret;  
  
  /* Obtain system tick value in milliseconds, and convert it to seconds. */    
  val = HAL_GetTick();
  val = val/1000;
  
  /* create a time[] array to pass as last argument of aci_gatt_update_char_value() API*/
  const uint8_t time[4] = {(val >> 24)&0xFF, (val >> 16)&0xFF, (val >> 8)&0xFF, (val)&0xFF};  
  
  /*
   * Update value of "Seconds characteristic" using aci_gatt_update_char_value() API 
   * Please refer to 'BlueNRG Application Command Interface.pdf' for detailed
   * API description   
   */
  ret = aci_gatt_update_char_value(timeServHandle, secondsCharHandle, 0, 4,
                                   time);
	
  if (ret != BLE_STATUS_SUCCESS){
    PRINTF("Error while updating TIME characteristic.\n") ;
    return BLE_STATUS_ERROR ;
  }
  return BLE_STATUS_SUCCESS;		
}

/**
 * @brief  Send a notification for a minute characteristic of time service.
 * @param  None
 * @retval Status
 */
tBleStatus Minutes_Notify(void)
{  
  uint32_t val; 
  uint32_t minuteValue;   
  tBleStatus ret;

  /* Obtain system tick value in milliseconds */  
  val = HAL_GetTick();	
  
  /* update "Minutes characteristic" value iff it has changed w.r.t. previous 
   * "minute" value. 
   */
  if((minuteValue=val/(60*1000))!=previousMinuteValue) {    
    /* memorize this "minute" value for future usage */
    previousMinuteValue = minuteValue;    
  
    /* create a time[] array to pass as last argument of aci_gatt_update_char_value() API*/
    const uint8_t time[4] = {(minuteValue >> 24)&0xFF, (minuteValue >> 16)&0xFF, (minuteValue >> 8)&0xFF, (minuteValue)&0xFF};
   
  /*
   * Update value of "Minutes characteristic" using aci_gatt_update_char_value() API 
   * Please refer to 'BlueNRG Application Command Interface.pdf' for detailed
   * API description   
   */    
    ret = aci_gatt_update_char_value(timeServHandle, minuteCharHandle, 0, 4,
                                     time);          
    if (ret != BLE_STATUS_SUCCESS){
      PRINTF("Error while updating TIME characteristic.\n") ;
      return BLE_STATUS_ERROR ;
    }
  }
  return BLE_STATUS_SUCCESS;	
}

/**
 * @brief  Updates "Seconds and Minutes characteristics" values.
 * @param  None
 * @retval None
 */
void Update_Time_Characteristics(void) {
  /* update "seconds and minutes characteristics" of time service */
  Seconds_Update();
  Minutes_Notify();
}

/*
 * @brief  Add LED button service using a vendor specific profile.
 * @param  None
 * @retval Status
 */
tBleStatus Add_LED_Service(void)
{
  tBleStatus ret;
  uint8_t uuid[16];
  
  /* copy "LED service UUID" defined above to 'uuid' local variable */
  COPY_LED_SERVICE_UUID(uuid);
  /* 
   * now add "LED service" to GATT server, service handle is returned
   * via 'ledServHandle' parameter of aci_gatt_add_serv() API. 
   * Please refer to 'BlueNRG Application Command Interface.pdf' for detailed
   * API description 
  */  
  ret = aci_gatt_add_serv(UUID_TYPE_128, uuid, PRIMARY_SERVICE, 7,
                          &ledServHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;    
  
  /* copy "LED button characteristic UUID" defined above to 'uuid' local variable */  
  COPY_LED_UUID(uuid);
  /* 
   * now add "LED button characteristic" to LED service, characteristic handle 
   * is returned via 'ledButtonCharHandle' parameter of aci_gatt_add_char() API.
   * This characteristic is writable, as specified by 'CHAR_PROP_WRITE' parameter.
   * Please refer to 'BlueNRG Application Command Interface.pdf' for detailed
   * API description 
  */   
  ret =  aci_gatt_add_char(ledServHandle, UUID_TYPE_128, uuid, 4,
                           CHAR_PROP_WRITE | CHAR_PROP_WRITE_WITHOUT_RESP, ATTR_PERMISSION_NONE, GATT_NOTIFY_ATTRIBUTE_WRITE,
                           16, 1, &ledButtonCharHandle);
  if (ret != BLE_STATUS_SUCCESS) goto fail;  
  
  PRINTF("Service LED BUTTON added. Handle 0x%04X, LED button Charac handle: 0x%04X\n",ledServHandle, ledButtonCharHandle);	
  return BLE_STATUS_SUCCESS; 
  
fail:
  PRINTF("Error while adding LED service.\n");
  return BLE_STATUS_ERROR;
}

/**
 * @brief  This function is called attribute value corresponding to 
 *         ledButtonCharHandle characteristic gets modified.
 * @param  Handle of the attribute
 * @param  Size of the modified attribute data
 * @param  Pointer to the modified attribute data
 * @retval None
 */
void Attribute_Modified_CB(uint16_t handle, uint8_t data_length, uint8_t *att_data)
{
  /* If GATT client has modified 'LED button characteristic' value, toggle LED2 */
  if(handle == ledButtonCharHandle + 1){      
      BSP_LED_Toggle(LED2);
  }
}
#endif /* NEW_SERVICES */
/**
 * @}
 */
 
/**
 * @}
 */

/**
 * @}
 */

 /**
 * @}
 */
 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
