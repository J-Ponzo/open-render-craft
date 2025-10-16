extends Node

var mock : ORCTEST_GDOverridableMacro_Mock

func before_each():
	mock = ORCTEST_GDOverridableMacro_Mock.new()
