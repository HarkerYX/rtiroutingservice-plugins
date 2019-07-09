/*
 * (c) 2019 Copyright, Real-Time Innovations, Inc.  All rights reserved.
 *
 * RTI grants Licensee a license to use, modify, compile, and create derivative
 * works of the Software.  Licensee has the right to distribute object form
 * only for use with RTI products.  The Software is provided "as is", with no
 * warranty of any type, including any warranty for fitness for any purpose.
 * RTI is under no obligation to maintain or support the Software.  RTI shall
 * not be liable for any incidental or consequential damages arising out of the
 * use or inability to use the software.
 */

/**
 * @file rtiadapt_mqtt_client.h
 * @brief Header file for the MQTT Client API used by the MQTT Adapter.
 * 
 * This header file defines a simple MQTT Client API.
 * 
 */

#ifndef rtiadapt_mqtt_client_h
#define rtiadapt_mqtt_client_h

/*****************************************************************************
                          MQTT Client Library Selection
 *****************************************************************************/

/**
 * @addtogroup RtiMqtt_Compiler
 * @{
 */

/**
 * @defgroup RtiMqtt_Client_Library MQTT Library defines
 * 
 * The MQTT Adapter relies on an external MQTT Client Library to provide the
 * basic MQTT functionalities required to connect to an MQTT Broker, and to
 * exchange data with it.
 * 
 * By default, the Paho Asynchronous C API will be used.
 * 
 * Currently no other MQTT Client Library is supported, but the Mosquitto
 * Client API might be supported soon.
 * 
 * @addtogroup RtiMqtt_Client_Library
 * @{
 */

/**
 * @brief Macro constant identifying the "Paho Asynchronous C API"
 */
#define MQTT_CLIENT_API_PAHO_C          1
/**
 * @brief Macro constant identifying the "Mosquitto Client API"
 */
#define MQTT_CLIENT_API_MOSQUITTO       2
/**
 * @brief Macro constant defining the default MQTT Client Library.
 */
#define MQTT_CLIENT_API_DEFAULT         MQTT_CLIENT_API_PAHO_C


#ifndef MQTT_CLIENT_API
/**
 * @ingroup RtiMqtt_Compiler_User
 * @brief A macro which can be used to guard code based on the MQTT Client
 * Library in use.
 */
#define MQTT_CLIENT_API                 MQTT_CLIENT_API_DEFAULT
#endif

/** @} */

/** @} */

/*****************************************************************************
 *                   MQTT Client
 *****************************************************************************/
/**
 * @addtogroup RtiMqtt_Client
 * @{
 */

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief An MQTT client that will connect to a Broker and exchange data with 
 * it.
 */
struct RTI_MQTT_Client;

/**
 * @brief An abstraction of one or more MQTT subscriptions created by an
 * `RTI_MQTT_Client` on its MQTT Broker.
 */
struct RTI_MQTT_Subscription;

/**
 * @brief An abstraction of the MQTT message generated by an `RTI_MQTT_Client`
 * which are published with a common set of configuration properties.
 */
struct RTI_MQTT_Publication;

/**
 * @brief A DDS data type modelling a generic MQTT message.
 */
struct RTI_MQTT_Message;


/**
 * @defgroup RtiMqtt_Client_Configuration MQTT Configuration
 * 
 * 
 * @addtogroup RtiMqtt_Client_Configuration
 * @{
 * 
 * @defgroup RtiMqtt_Client_Configuration_Client MQTT Client Configuration
 * @defgroup RtiMqtt_Client_Configuration_Subscription MQTT Subscription Configuration
 * @defgroup RtiMqtt_Client_Configuration_Publication MQTT Publication Configuration
 * @defgroup RtiMqtt_Client_Configuration_Infrastructure MQTT Configuration Infrastructure
 * 
 */

/**
 * @addtogroup RtiMqtt_Client_Configuration_Infrastructure
 * @{
 * 
 */

/**
 * @brief An `RTI_MQTT_Time` instance representing a zero duration or instant
 * in time.
 */
extern const RTI_MQTT_Time RTI_MQTT_Time_ZERO;

/**
 * @brief An `RTI_MQTT_Time` instance representing an infinite duration or 
 * an infinitely distant moment in time.
 */
extern const RTI_MQTT_Time RTI_MQTT_Time_INFINITE;

/**
 * @brief A macro that can be used to initialize static values of 
 * `RTI_MQTT_Time` with a certain seconds and nanoseconds components.
 */
#define RTI_MQTT_Time_INITIALIZER(sec_,nsec_)   { (sec_), (nsec_) }


#if RTI_MQTT_USE_STATIC_TYPES
#define RTI_MQTT_DYNAMIC_TYPES          DDS_BOOLEAN_FALSE
#else
/**
 * @ingroup RtiMqtt_Compiler_Auto
 * @brief Flag that controls the default configuration of 
 * `RTI_MQTT_Publication` and `RTI_MQTT_Subscription` with respect to the
 * internal use of `DDS_DynamicData` samples or statically typed data.
 */
#define RTI_MQTT_DYNAMIC_TYPES          DDS_BOOLEAN_TRUE
#endif /* RTI_MQTT_USE_STATIC_TYPES */

/** @} */

/**
 * @addtogroup RtiMqtt_Client_Configuration_Client
 * @{
 */

/**
 * @brief The default configuration of an `RTI_MQTT_Client` if no other
 * configuration property is specified.
 */
extern const RTI_MQTT_ClientConfig RTI_MQTT_ClientConfig_DEFAULT;

/**
 * @brief Default initializer for static values of `RTI_MQTT_ClientConfig`.
 */
#define RTI_MQTT_ClientConfig_INITIALIZER \
{ \
    "",                                 /* id */ \
    RTI_MQTT_MqttProtocolVersion_MQTT_DEFAULT, /* protocol_version */ \
    DDS_SEQUENCE_INITIALIZER,           /* server_uris */ \
    RTI_MQTT_Time_INITIALIZER(10,0),    /* connect_timeout */ \
    10,                                 /* max_connection_retries */ \
    RTI_MQTT_Time_INITIALIZER(10,0),    /* keep_alive_period */ \
    DDS_BOOLEAN_FALSE,                  /* clean_session */ \
    DDS_BOOLEAN_TRUE,                   /* unsubscribe_on_disconnect */ \
    RTI_MQTT_Time_INITIALIZER(3,0),     /* max_reply_timeout */ \
    DDS_BOOLEAN_TRUE,                   /* reconnect */ \
    10,                                 /* max_unack_messages */ \
    RTI_MQTT_PersistenceLevel_NONE,     /* persistence_level */ \
    NULL,                               /* persistence_storage */ \
    NULL,                               /* username */ \
    NULL,                               /* password */ \
    NULL                                /* ssl_tls_config */ \
}


/**
 * @brief Allocate or initialize an `RTI_MQTT_ClientConfig` with default
 * configuration properties.
 * 
 * @param config_out If a value is passed, the `RTI_MQTT_ClientConfig` 
 * will be initialized. Otherwise, `config_out` will be used to return the new
 * instance of `RTI_MQTT_ClientConfig` allocated by this function.
 * 
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_ClientConfig`
 * was successfully initialized with default properties (and possibly 
 * allocated), `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_ClientConfig_new RTI_MQTT_ClientConfig_delete
 */
DDS_ReturnCode_t
RTI_MQTT_ClientConfig_default(RTI_MQTT_ClientConfig **config_out);

/**
 * @brief Allocate and initialize a new `RTI_MQTT_ClientConfig`.
 * 
 * Each attribute will be initialize to the default value of its data type.
 * 
 * @param allocate_optional A flag controlling whether optional fiels of
 * `RTI_MQTT_ClientConfig` should also be allocated with a default value.
 * 
 * @param config_out The newly created `RTI_MQTT_ClientConfig` will be
 * returned through this paramter.
 * 
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_ClientConfig`
 * was successfully created, `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_ClientConfig_delete
 */
DDS_ReturnCode_t
RTI_MQTT_ClientConfig_new(DDS_Boolean allocate_optional,
                          RTI_MQTT_ClientConfig **config_out);

/**
 * @brief Delete an `RTI_MQTT_ClientConfig`.
 * 
 * @param self The `RTI_MQTT_ClientConfig` to delete.
 * 
 * @see RTI_MQTT_ClientConfig_new
 */
void
RTI_MQTT_ClientConfig_delete(RTI_MQTT_ClientConfig *self);

/** @} */

/**
 * @addtogroup RtiMqtt_Client_Configuration_Subscription
 * @{
 */

/**
 * @brief The default configuration of an `RTI_MQTT_Subscription` if no other
 * configuration property is specified.
 */
extern const RTI_MQTT_SubscriptionConfig RTI_MQTT_SubscriptionConfig_DEFAULT;

/**
 * @brief Default initializer for static values of 
 * `RTI_MQTT_SubscriptionConfig`.
 */
#define RTI_MQTT_SubscriptionConfig_INITIALIZER \
{ \
    DDS_SEQUENCE_INITIALIZER, /* topic_filters */ \
    RTI_MQTT_QosLevel_TWO,    /* max_qos */ \
    0                         /* message_queue_size */ \
}

/**
 * @brief Allocate or initialize an `RTI_MQTT_SubscriptionConfig` with default
 * configuration properties.
 * 
 * @param config_out If a value is passed, the `RTI_MQTT_SubscriptionConfig` 
 * will be initialized. Otherwise, `config_out` will be used to return the new
 * instance of `RTI_MQTT_SubscriptionConfig` allocated by this function.
 * 
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_SubscriptionConfig`
 * was successfully initialized with default properties (and possibly 
 * allocated), `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_SubscriptionConfig_new RTI_MQTT_SubscriptionConfig_delete
 */
DDS_ReturnCode_t
RTI_MQTT_SubscriptionConfig_default(RTI_MQTT_SubscriptionConfig **config_out);

/**
 * @brief Allocate and initialize a new `RTI_MQTT_SubscriptionConfig`.
 * 
 * Each attribute will be initialize to the default value of its data type.
 * 
 * @param allocate_optional A flag controlling whether optional fiels of
 * `RTI_MQTT_SubscriptionConfig` should also be allocated with a default value.
 * 
 * @param config_out The newly created `RTI_MQTT_SubscriptionConfig` will be
 * returned through this paramter.
 * 
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_SubscriptionConfig`
 * was successfully created, `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_SubscriptionConfig_delete
 */
DDS_ReturnCode_t
RTI_MQTT_SubscriptionConfig_new(DDS_Boolean allocate_optional,
                                RTI_MQTT_SubscriptionConfig **config_out);


/**
 * @brief Delete an `RTI_MQTT_SubscriptionConfig`.
 * 
 * @param self The `RTI_MQTT_SubscriptionConfig` to delete.
 * 
 * @see RTI_MQTT_SubscriptionConfig_new
 */
void
RTI_MQTT_SubscriptionConfig_delete(RTI_MQTT_SubscriptionConfig *self);


/** @} */

/**
 * @addtogroup RtiMqtt_Client_Configuration_Publication
 * @{
 */

/**
 * @brief The default configuration of an `RTI_MQTT_Publication` if no other
 * configuration property is specified.
 */
extern const RTI_MQTT_PublicationConfig RTI_MQTT_PublicationConfig_DEFAULT;

/**
 * @brief Default initializer for static values of 
 * `RTI_MQTT_PublicationConfig`.
 */
#define RTI_MQTT_PublicationConfig_INITIALIZER \
{ \
    "",                             /* topic */ \
    RTI_MQTT_QosLevel_ZERO,         /* qos */ \
    DDS_BOOLEAN_FALSE,              /* retained */ \
    DDS_BOOLEAN_FALSE,              /* use_message_info */ \
    RTI_MQTT_Time_INITIALIZER(10,0) /* max_wait_time */ \
}

/**
 * @brief Allocate or initialize an `RTI_MQTT_PublicationConfig` with default
 * configuration properties.
 * 
 * @param config_out If a value is passed, the `RTI_MQTT_PublicationConfig` 
 * will be initialized. Otherwise, `config_out` will be used to return the new
 * instance of `RTI_MQTT_PublicationConfig` allocated by this function.
 * 
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_PublicationConfig`
 * was successfully initialized with default properties (and possibly 
 * allocated), `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_PublicationConfig_new RTI_MQTT_PublicationConfig_delete
 */
DDS_ReturnCode_t
RTI_MQTT_PublicationConfig_default(RTI_MQTT_PublicationConfig **config_out);

/**
 * @brief Allocate and initialize a new `RTI_MQTT_PublicationConfig`.
 * 
 * Each attribute will be initialize to the default value of its data type.
 * 
 * @param allocate_optional A flag controlling whether optional fiels of
 * `RTI_MQTT_PublicationConfig` should also be allocated with a default value.
 * 
 * @param config_out The newly created `RTI_MQTT_PublicationConfig` will be
 * returned through this paramter.
 * 
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_PublicationConfig`
 * was successfully created, `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_PublicationConfig_delete
 */
DDS_ReturnCode_t
RTI_MQTT_PublicationConfig_new(DDS_Boolean allocate_optional,
                               RTI_MQTT_PublicationConfig **config_out);

/**
 * @brief Delete an `RTI_MQTT_PublicationConfig`.
 * 
 * @param self The `RTI_MQTT_PublicationConfig` to delete.
 * 
 * @see RTI_MQTT_PublicationConfig_new
 */
void
RTI_MQTT_PublicationConfig_delete(RTI_MQTT_PublicationConfig *self);

/**
 * @brief Default initializer for static values of 
 * `RTI_MQTT_WriteParams`.
 */
#define RTI_MQTT_WriteParams_INITIALIZER \
{ \
    RTI_MQTT_QosLevel_ZERO,         /* qos_level */ \
    DDS_BOOLEAN_FALSE               /* retained */ \
}

/** @} */


/** @} */

/**
 * @defgroup RtiMqtt_Client_Client MQTT Client
 * 
 * @addtogroup RtiMqtt_Client_Client
 * @{
 */

/**
 * @brief Allocate and initialize a new `RTI_MQTT_Client` from the specified
 * configuration.
 * 
 * The new client will be created in `DISCONNECTED` state.
 * 
 * @param config the configuration for the new `RTI_MQTT_Client`.
 * @param client_out The new `RTI_MQTT_Client` upon success, or `NULL` on
 * failure.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_Client` was
 * successfully allocated and initialized, `DDS_RETCODE_ERROR` if an error
 * occurred.
 * 
 * @see RTI_MQTT_Client_delete
 */
DDS_ReturnCode_t
RTI_MQTT_Client_new(RTI_MQTT_ClientConfig *config,
                      struct RTI_MQTT_Client **client_out);

/**
 * @brief Delete and existing `RTI_MQTT_Client`.
 * 
 * The client will be destroyed without disconnecting from the Broker.
 * 
 * @param self the `RTI_MQTT_Client` to delete.
 * 
 * @see RTI_MQTT_Client_new
 */
void
RTI_MQTT_Client_delete(struct RTI_MQTT_Client *self);

/**
 * @brief Establish a connection between an `RTI_MQTT_Client` and one of its
 * MQTT Brokers.
 * 
 * The `RTI_MQTT_Client` will transition into `CONNECTING` state, and it will
 * try to establish a connection with the first Broker in the list stored by
 * its configuration.
 * 
 * If a connection cannot be established within the configured maximum timeout,
 * this function will fail, and the `RTI_MQTT_Client` will transition to
 * `DISCONNECTED` state.
 * 
 * Once a connection has been successfully established, the `RTI_MQTT_Client`
 * will transition to `CONNECTED` state.
 * 
 * @param self the `RTI_MQTT_Client` to connect
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if a connection was successfully
 * established, or `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Client_disconnect
 */
DDS_ReturnCode_t
RTI_MQTT_Client_connect(struct RTI_MQTT_Client *self);

/**
 * @brief Terminate the connection between an `RTI_MQTT_Client` and its MQTT 
 * Broker.
 * 
 * If the client is in `CONNECTED` state, this operation will send a 
 * disconnection request to the associated MQTT Broker, and wait for a reply
 * to guarantee a clean disconnection.
 * 
 * If requested by configuration, all subscriptions currently owned by the
 * `RTI_MQTT_Client` will be deleted on the MQTT Broker before disconnection.
 * 
 * If the client is not in `CONNECTED` state, this operation will do nothing.
 * 
 * @param self the `RTI_MQTT_Client` to disconnect.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the connection was successfully
 * terminated, or `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Client_connect
 */
DDS_ReturnCode_t
RTI_MQTT_Client_disconnect(struct RTI_MQTT_Client *self);

/**
 * @brief Create a new subscription to MQTT data.
 * 
 * This operation will create a new `RTI_MQTT_Subscription` owned by the
 * `RTI_MQTT_Client`, to represent one or more subscriptions on the 
 * associated MQTT Broker.
 * 
 * The `RTI_MQTT_Client` will request the creation of new subscription of 
 * the MQTT Broker based on the specified configuration.
 * 
 * This operation will fail if the `RTI_MQTT_Client` is not in `CONNECTED`
 * state.
 * 
 * @param self the `RTI_MQTT_Client` for which a subscription is to be created.
 * @param config  the configuration of the new `RTI_MQTT_Subscription`
 * @param sub_out the new `RTI_MQTT_Subscription` upon success, or `NULL` upon
 * failure.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the new subscription was
 * successfully created, both locally and on the MQTT Broker, 
 * `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Client_unsubscribe
 */
DDS_ReturnCode_t
RTI_MQTT_Client_subscribe(struct RTI_MQTT_Client *self,
                            RTI_MQTT_SubscriptionConfig *config,
                            struct RTI_MQTT_Subscription **sub_out);

/**
 * @brief Delete an existing subscription to MQTT data.
 * 
 * This operation will delete an existing `RTI_MQTT_Subscription` owned by
 * the `RTI_MQTT_Client`.
 * 
 * The `RTI_MQTT_Client` will delete all subscriptions on the MQTT Broker that
 * match the topic filters of the `RTI_MQTT_Subscription`.
 * 
 * This operation will fail if the `RTI_MQTT_Client` is not in `CONNECTED`
 * state.
 * 
 * @param self the `RTI_MQTT_Client` that created the subscription
 * @param sub the `RTI_MQTT_Subscription` to delete.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the subscription was 
 * successfully deleted, both locally and on the MQTT Broker, 
 * `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Client_subscribe
 */
DDS_ReturnCode_t
RTI_MQTT_Client_unsubscribe(struct RTI_MQTT_Client *self,
                              struct RTI_MQTT_Subscription *sub);

/**
 * @brief Create a new output stream of MQTT data.
 * 
 * This operation will create a new `RTI_MQTT_Publication` owned by the
 * `RTI_MQTT_Client`, which represents a stream of MQTT messages related
 * either logically, or by means of common "topic", "qos", and "retained"
 * configuration.
 * 
 * The creation of an `RTI_MQTT_Publication` is a local operation which 
 * doesn't require any communication with the MQTT Broker.
 * 
 * Nonetheless, for symmetry with the creation of `RTI_MQTT_Subscription`
 * objects, this operation will fail if the `RTI_MQTT_Client` is not in 
 * `CONNECTED` state.
 * 
 * @param self the `RTI_MQTT_Client` for which a publication is to be created.
 * @param config the configuration of the new publication.
 * @param pub_out the new `RTI_MQTT_Publication` upon success, or `NULL` upon
 * failure.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the new `RTI_MQTT_Publication`
 * was successfully created, `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Client_unpublish
 */
DDS_ReturnCode_t
RTI_MQTT_Client_publish(struct RTI_MQTT_Client *self,
                          RTI_MQTT_PublicationConfig *config,
                          struct RTI_MQTT_Publication **pub_out);

/**
 * @brief Delete an existing output stream of MQTT data.
 * 
 * This operation will delete an existing `RTI_MQTT_Publication` owned by the
 * `RTI_MQTT_Client`.
 * 
 * The deletion of an `RTI_MQTT_Publication` is a local operation which 
 * doesn't require any communication with the MQTT Broker.
 * 
 * Nonetheless, for symmetry with the deletion of `RTI_MQTT_Subscription`
 * objects, this operation will fail if the `RTI_MQTT_Client` is not in 
 * `CONNECTED` state.
 * 
 * @param self the `RTI_MQTT_Client` that created the publication.
 * @param pub the `RTI_MQTT_Publication` to delete.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the `RTI_MQTT_Publication` was
 * successfully deleted, `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Client_publish
 */
DDS_ReturnCode_t
RTI_MQTT_Client_unpublish(struct RTI_MQTT_Client *self,
                            struct RTI_MQTT_Publication *pub);

/**
 * @brief Access the MQTT identifier of an `RTI_MQTT_Client`.
 * 
 * This value comes from the associated configuration object.
 * 
 * @param self an `RTI_MQTT_Client`
 * @return const char* the MQTT identifier used by the `RTI_MQTT_Client`
 * when connecting to an MQTT Broker.
 * 
 * @see RTI_MQTT_ClientConfig_default RTI_MQTT_PROPERTY_CLIENT_ID
 */
const char *
RTI_MQTT_Client_get_id(struct RTI_MQTT_Client *self);

/** @} */

/**
 * @defgroup RtiMqtt_Client_Subscription MQTT Subscription
 * 
 * @addtogroup RtiMqtt_Client_Subscription
 * @{
 */

/**
 * @brief Prototype of callback to receive notification of new data received
 * on an `RTI_MQTT_Subscription`.
 * 
 * @param listener_data A "state object" that was specified when the 
 * callback was registered on the `RTI_MQTT_Subscription`.
 * 
 * @param sub The `RTI_MQTT_Subscription` with new data available.
 * 
 */
typedef void (*RTI_MQTT_Subscription_DataAvailableCallback)(
            void *listener_data,
            struct RTI_MQTT_Subscription *sub);

/**
 * @brief Set the listener callback that will be notified by the 
 * `RTI_MQTT_Subscription` when it has new data available.
 * 
 * @param self the `RTI_MQTT_Subscription` on which to set the lister
 * @param listener a callback that will be called every time the
 * `RTI_MQTT_Subscription` has new data available.
 * @param listener_data  An optional "state object" that will be passed to the
 * listener callback every time it is invoked.
 * @return DDS_ReturnCode_t `DDs_RETCODE_OK` if the listener was successfully
 * set, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Subscription_set_data_available_listener(
    struct RTI_MQTT_Subscription *self,
    RTI_MQTT_Subscription_DataAvailableCallback listener,
    void *listener_data);

/**
 * @brief A constant that can be used to read all available messages from
 * an `RTI_MQTT_Subscription`.
 */
#define RTI_MQTT_SUBSCRIPTION_READ_LENGTH_UNLIMITED         0

/**
 * @brief Read available messages as `DDS_DynamicData` samples.
 * 
 * If @ref RTI_MQTT_SUBSCRIPTION_READ_LENGTH_UNLIMITED is used (and a buffer
 * is loaned by the `RTI_MQTT_Subscription`), no other read may be performed 
 * until @ref RTI_MQTT_Subscription_return_loan_dynamic is called.
 * 
 * @param self The `RTI_MQTT_Subscription` from which to read messages.
 * @param max_messages Maximum number of new messages to read. 
 * @ref RTI_MQTT_SUBSCRIPTION_READ_LENGTH_UNLIMITED may be used to read all
 * available messages.
 * @param messages A sequence of size `max_messages` where new messages will
 * be read. If @ref RTI_MQTT_SUBSCRIPTION_READ_LENGTH_UNLIMITED is used, then
 * the sequence must not have a buffer allocated, as it will be loaned one 
 * by the `RTI_MQTT_Subscription`.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if new messages were read 
 * successfully, `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Subscription_return_loan
 *      RTI_MQTT_SUBSCRIPTION_READ_LENGTH_UNLIMITED
 * RTI_MQTT_Subscription_read
 */
DDS_ReturnCode_t
RTI_MQTT_Subscription_read(
    struct RTI_MQTT_Subscription *self,
    DDS_UnsignedLong max_messages,
    struct DDS_DynamicDataSeq *messages);

/**
 * @brief Return `DDS_DynamicData` samples read from an 
 * `RTI_MQTT_Subscription`.
 * 
 * This function must be called after calling 
 * @ref RTI_MQTT_Subscription_read with 
 * @ref RTI_MQTT_SUBSCRIPTION_READ_LENGTH_UNLIMITED
 * 
 * @param self the `RTI_MQTT_Subscription` from which the messages were read.
 * @param messages The messages to return.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the messages were successfully
 * return, `DDS_RETCODE_ERROR` otherwise.
 * 
 * @see RTI_MQTT_Subscription_read
 */
DDS_ReturnCode_t
RTI_MQTT_Subscription_return_loan(
    struct RTI_MQTT_Subscription *self,
    struct DDS_DynamicDataSeq *messages);

/** @} */

/**
 * @defgroup RtiMqtt_Client_Publication MQTT Publication
 * 
 * @addtogroup RtiMqtt_Client_Publication
 * @{
 */

/**
 * @brief Write an MQTT message from a `DDS_DynamicData` sample.
 * 
 * The payload of the specified message will be published to MQTT.
 * 
 * The "publication settings" (e.g. topic, qos, retained flag...) will be 
 * either determined from the `RTI_MQTT_Publication`'s configuration, of from
 * `message`'s `info` field, if `use_message_info` is enabled. In this case,
 * the operatio will fail if `message.info` is empty.
 * 
 * @param self the `RTI_MQTT_Publication` used to write the MQTT message.
 * @param message The message to write.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the message was successfully
 * published to the associated MQTT Broker, `DDS_RETCODE_ERROR` otherwise.
 * 
 */
DDS_ReturnCode_t
RTI_MQTT_Publication_write(
    struct RTI_MQTT_Publication *self,
    DDS_DynamicData *message);

/**
 * @brief Write an MQTT message from a raw buffer, using custom write
 * parameters.
 * 
 * This operation will bypass the `RTI_MQTT_Publication`'s configuration and
 * publish the specified `buffer` to MQTT using the provided "publication
 * settings".
 * 
 * @param self the `RTI_MQTT_Publication` used to write the MQTT message.
 * @param buffer The buffer to write.
 * @param buffer_len Size of `buffer`.
 * @param topic MQTT topic to which the message will be published.
 * @param params Publication settings for writing the message to MQTT.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the message was successfully
 * published to the associated MQTT Broker, `DDS_RETCODE_ERROR` otherwise.
 * 
 */
DDS_ReturnCode_t
RTI_MQTT_Publication_write_w_params(
    struct RTI_MQTT_Publication *self,
    const char *buffer,
    DDS_UnsignedLong buffer_len,
    const char *topic,
    RTI_MQTT_WriteParams *params);

/** @} */

/**
 * @defgroup RtiMqtt_Client_Message MQTT Message
 * 
 * @addtogroup RtiMqtt_Client_Message
 * @{
 */

/**
 * @brief Convert an `RTI_MQTT_Message` sample into a `DDS_DynamicData`
 * sample.
 * 
 * @param self The `RTI_MQTT_Message` to convert.
 * @param sample The `DDS_DynamicData` sample that will be set.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the sample was successfully
 * converted, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Message_to_dynamic_data(RTI_MQTT_Message *self,
                                 DDS_DynamicData *sample);

/**
 * @brief Convert a `DDS_DynamicData` sample into an `RTI_MQTT_Message`
 * sample.
 * 
 * @param self The `RTI_MQTT_Message` that will be set.
 * @param sample The `DDS_DynamicData` to convert.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the sample was successfully
 * converted, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Message_from_dynamic_data(RTI_MQTT_Message *self,
                                   DDS_DynamicData *sample);


/** @} */



/****************************************************************************/
/*                              Thread utilities                            */
/****************************************************************************/


/**
 * @defgroup RtiMqtt_Client_Platform Platform Services
 * 
 * @addtogroup RtiMqtt_Client_Platform
 * @{
 */

/**
 * @brief Prototype of a thread function for @ref RTI_MQTT_Thread_spawn.
 * 
 */
typedef void* (*RTI_MQTT_ThreadFn)(void *arg);

/**
 * @brief A portable abstraction for spawing user threads.
 * 
 * @param thread function to be executed by the thread
 * @param arg optional argument that will be passed to the thread function
 * @param handle_out If specified, a handle for the new thread will be 
 * returned via this parameter.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the thread was created
 * and started successfully, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Thread_spawn(
    RTI_MQTT_ThreadFn thread, 
    void *arg,
    void **handle_out);

/**
 * @brief Join a thread and wait for its completion.
 * 
 * @param handle A handle for the thread to join.
 * @param result_out The result value returned by the thread function.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the thread joined successfully,
 * `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Thread_join(void *handle, void **result_out);

/****************************************************************************/
/*                              Mutex utilities                            */
/****************************************************************************/

/**
 * @brief Native type to represent a mutex on POSIX platforms.
 * 
 */
#define RTI_MQTT_MUTEX_POSIX                pthread_mutex_t

#define RTI_MQTT_MUTEX_INITIALIZER_POSIX    PTHREAD_MUTEX_INITIALIZER

/**
 * @brief Native type to represent a mutex on Windows platforms.
 * 
 */
#define RTI_MQTT_MUTEX_WINDOWS              HANDLE

#define RTI_MQTT_MUTEX_INITIALIZER_WINDOWS  NULL

#if RTI_MQTT_PLATFORM == RTI_MQTT_PLATFORM_POSIX
/**
 * @brief Native type to represent a mutex on current build platform.
 * 
 */
#define RTI_MQTT_MUTEX_NATIVE               RTI_MQTT_MUTEX_POSIX
#define RTI_MQTT_MUTEX_INITIALIZER_NATIVE   RTI_MQTT_MUTEX_INITIALIZER_POSIX

#elif RTI_MQTT_PLATFORM == RTI_MQTT_PLATFORM_WINDOWS
#define RTI_MQTT_MUTEX_NATIVE               RTI_MQTT_MUTEX_WINDOWS
#define RTI_MQTT_MUTEX_INITIALIZER_NATIVE   RTI_MQTT_MUTEX_INITIALIZER_WINDOWS
#endif

/**
 * @brief Mutex type for current build platform.
 * 
 */
typedef RTI_MQTT_MUTEX_NATIVE RTI_MQTT_Mutex;

#define RTI_MQTT_Mutex_INITIALIZER          RTI_MQTT_MUTEX_INITIALIZER_NATIVE

/**
 * @brief Initialize a new mutex.
 * 
 * @param self The mutex to initialize
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the mutex was successfully
 * initialized, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Mutex_initialize(RTI_MQTT_Mutex *self);

/**
 * @brief Finalize an existing mutex
 * 
 * @param self The mutex to finalize
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the mutex was successfully
 * finalized, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Mutex_finalize(RTI_MQTT_Mutex *self);

/**
 * @brief Acquire ownership of a mutex.
 * 
 * @param self The mutex to acquire.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the mutex was successfully
 * acquired, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Mutex_take(RTI_MQTT_Mutex *self);

/**
 * @brief Release ownership of a mutex.
 * 
 * @param self The mutex to relese.
 * @return DDS_ReturnCode_t `DDS_RETCODE_OK` if the mutex was successfully
 * released, `DDS_RETCODE_ERROR` otherwise.
 */
DDS_ReturnCode_t
RTI_MQTT_Mutex_give(RTI_MQTT_Mutex *self);

/**
 * @brief A macro to call RTI_MQTT_Mutex_take and abort the process if
 * the operation fails.
 */
#define RTI_MQTT_Mutex_assert(m_) \
{\
    RTI_MQTT_TRACE_1("ASSERT mutex:","%p",(m_))\
    if (DDS_RETCODE_OK != RTI_MQTT_Mutex_take((m_))) \
    { \
        RTI_MQTT_ERROR_1("failed to assert mutex:","mutex=%p",(m_)) \
        abort(); \
    } \
}

/**
 * @brief A macro to call RTI_MQTT_Mutex_give and abort the process if
 * the operation fails
 */
#define RTI_MQTT_Mutex_release(m_) \
{\
    RTI_MQTT_TRACE_1("RELEASE mutex:","%p",(m_))\
    if (DDS_RETCODE_OK != RTI_MQTT_Mutex_give((m_))) \
    { \
        RTI_MQTT_ERROR_1("failed to release mutex:","mutex=%p",(m_)) \
        abort(); \
    }\
}\

/**
 * @brief A macro to call RTI_MQTT_Mutex_take, abort the process if
 * the operation fails, and store the current state (locked or not locked)
 * in a local variable.
 */
#define RTI_MQTT_Mutex_assert_w_state(m_,l_) \
{\
    RTI_MQTT_TRACE_2("ASSERT mutex_w_state:","%p (%d)",(m_),*(l_))\
    RTI_MQTT_Mutex_assert(m_);\
    *(l_) = DDS_BOOLEAN_TRUE; \
}

/**
 * @brief A macro to call RTI_MQTT_Mutex_give, abort the process if
 * the operation fails, and store the current state (locked or not locked)
 * in a local variable.
 */
#define RTI_MQTT_Mutex_release_w_state(m_,l_) \
{\
    RTI_MQTT_TRACE_2("RELEASE mutex_w_state:","%p (%d)",(m_),*(l_))\
    *(l_) = DDS_BOOLEAN_FALSE; \
    RTI_MQTT_Mutex_release(m_);\
}

/**
 * @brief A macro to call RTI_MQTT_Mutex_release if the specified
 * state variable is true.
 */
#define RTI_MQTT_Mutex_release_from_state(m_,l_) \
{\
    RTI_MQTT_TRACE_2("RELEASE mutex_from_state:","%p (%d)",(m_),*(l_))\
    if (*(l_)) \
    {\
        RTI_MQTT_Mutex_release_w_state(m_,l_);\
    } \
}

/** @} */

/**
 * @defgroup RtiMqtt_Client_IDL IDL Types
 * @addtogroup RtiMqtt_Client_IDL
 * @{
 * 
 * @defgroup RtiMqtt_Client_IDL_Config  MQTT Configuration
 * @defgroup RtiMqtt_Client_IDL_Message MQTT Message
 * @defgroup RtiMqtt_Client_IDL_Client  MQTT Client
 * @defgroup RtiMqtt_Client_IDL_Subscription  MQTT Subscription
 * @defgroup RtiMqtt_Client_IDL_Publication  MQTT Publication
 * 
 * @}
 */

#ifdef __cplusplus
    } /* extern "C" */
#endif


#endif /* rtiadapt_mqtt_client_h */

/** @} */