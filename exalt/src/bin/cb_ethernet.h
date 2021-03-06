#ifndef  CB_FUNCTIONS_INC
#define  CB_FUNCTIONS_INC

#include "daemon.h"
#include <Exalt.h>

DBusMessage * dbus_cb_eth_wireless_list_get(E_DBus_Object *obj __UNUSED__, DBusMessage *msg, int is_wireless);

DBusMessage * dbus_cb_eth_ip_get(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_eth_netmask_get(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_eth_gateway_get(E_DBus_Object *obj, DBusMessage *msg);

DBusMessage * dbus_cb_eth_list_get(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_eth_all_disconnected_is(E_DBus_Object *obj, DBusMessage *msg);

DBusMessage * dbus_cb_eth_wireless_is(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_eth_link_is(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_eth_up_is(E_DBus_Object *obj, DBusMessage *msg);
DBusMessage * dbus_cb_eth_dhcp_is(E_DBus_Object *obj, DBusMessage *msg);

DBusMessage * dbus_cb_eth_connected_is(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);

DBusMessage * dbus_cb_eth_up(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_eth_down(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);


DBusMessage * dbus_cb_eth_conf_apply(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);

DBusMessage * dbus_cb_eth_cmd_get(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);
DBusMessage * dbus_cb_eth_cmd_set(E_DBus_Object *obj __UNUSED__, DBusMessage *msg);

#endif   /* ----- #ifndef CB_FUNCTIONS_INC  ----- */

