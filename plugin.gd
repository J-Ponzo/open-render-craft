@tool
extends EditorPlugin
class_name ORC_Plugin

const AUTOLOAD_NAME = "ORC"
const AUTOLOAD_PATH = "res://addons/open-render-craft/scripts/ORC.gd"

const  MSG_PLUGIN_STARTED = "Plugin started"
const  MSG_PLUGIN_ENABLED = "Plugin enabled"
const  MSG_PLUGIN_DISABLED = "Plugin disabled"
const  MSG_PLUGIN_STOPPED = "Plugin stopped"

static func print(msg : String):
	print("ORC : %s" % msg)

static func push_warning(msg : String):
	push_warning("ORC : %s" % msg)

static func push_error(msg : String):
	push_error("ORC : %s" % msg)

func _enter_tree() -> void:
	ORC_Plugin.print(MSG_PLUGIN_ENABLED)
	add_autoload_singleton(AUTOLOAD_NAME, AUTOLOAD_PATH)
	ORC_Plugin.print(MSG_PLUGIN_STARTED)
	
func _exit_tree() -> void:
	ORC_Plugin.print(MSG_PLUGIN_DISABLED)
	remove_autoload_singleton(AUTOLOAD_NAME)
	ORC_Plugin.print(MSG_PLUGIN_STOPPED)
