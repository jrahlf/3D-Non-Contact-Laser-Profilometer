#!/usr/bin/env python
# -*- coding: utf-8 -*-

import copy

import utils
import action
import xml_utils
from parser_exception import ParserException

class ComponentDictionary(utils.SingleAssignDictionary):
	
	def __init__(self,  name):
		utils.SingleAssignDictionary.__init__(self, name)
		
		self.actions = utils.SortedDictionary()
		self.subscriptions = {}
	
	def updateIndex(self):
		""" Update the list of all actions and checks for duplicate identifiers """
		for component in self.values():
			for element in component.flattened().actions:
				if element.name in self.actions:
					other = self.actions[element.name]
					if element.id != other.id:
						raise ParserException("%s '%s' defined twice with " \
							"different identifiers!" % (self.name, element.name))
				self.actions[element.name] = element
			
			for event in component.flattened().events.subscribe:
				# append new events to the list
				key = event.name
				componentList = self.subscriptions.get(key, [])
				componentList.append(component)
				self.subscriptions[key] = componentList
		
		# check for duplicate identifiers
		actionIds = {}
		for action in self.actions:
			id = action.id
			if id in actionIds:
				next_id = id
				while next_id in actionIds:
					next_id += 1
				
				raise ParserException("Duplicate Action-Identifier, '0x%02x' is used for '%s' and '%s'!" \
						"'0x%02x' might be unused."	% 
						(id, action.name, actionIds[id].name, next_id))
			else:
				actionIds[id] = action
		
		# check that the component identifier is unique
		componentIdentifier = {}
		for component in self.values():
			id = component.flattened().id
			
			if id is None:
				continue
			
			if id in componentIdentifier:
				next_id = id
				while next_id in componentIdentifier:
					next_id += 1
				
				raise ParserException("Duplicate Component-Identifier, '0x%02x' is used for '%s' and '%s'! " \
						"'0x%02x' might be unused."	% 
						(id, component.name, componentIdentifier[id].name, next_id))
			else:
				componentIdentifier[id] = component
	
	def __iter__(self):
		""" Generates an iterator that will iterate over all non abstract components """
		return self.iter(None)
	
	def iter(self, abstract=False):
		"""
		Generate an iterator.
		
		Keywords arguments:
		abstract	--	Handling of abstract components (components
						that are not used in any container). For None it
						will iterate over all components, for False
						(default value) only over the none-abstract and for
						True accordingly only over the abstract.
		"""
		class ComponentIterator:
			def __init__(self, list, abstract):
				self.list = list
				self.list.sort()
				self.abstract = abstract
			
			def __iter__(self):
				return self
			
			def next(self):
				try:
					while True:
						item = self.list.pop(0)
						if self.abstract != None and item.abstract != self.abstract:
							continue
						return item
				except IndexError:
					raise StopIteration()
		
		return ComponentIterator(self.values(), abstract)

class EventContainer(object):
	"""
	
	Attributes:
	publish		--	All events the component will publish
	subscribe	--	All events the component will receive
	"""
	def __init__(self):
		self.publish = utils.SingleAssignDictionary("event::publish")
		self.subscribe = utils.SingleAssignDictionary("event::subscribe")
	
	def update(self, other):
		self.publish.update(other.publish)
		self.subscribe.update(other.subscribe)
	
	def copy(self):
		""" Create a shallow copy """
		c = copy.copy(self)
		c.publish = self.publish.copy()
		c.subscribe = self.subscribe.copy()
		return c
	
	def __str__(self):
		return "event::publish:\n%s\n"\
			   "event::subscribe:\n%s" % (str(self.publish), str(self.subscribe))

class Component(object):
	""" Representation of a component.
	
	Attributes:
	name		--	Name of the component
	description	--	Description string
	id			--	Identifier of the component (0..255). Must be set for
					any non abstract component. Can be 'None' for abstract
					components.
	extends		--	Super component. 'None' if no super component present.
	abstract	--	'False' if this component is used in any container. Default
					value is 'True', might be set to 'False' when parsing
					the containers.
	actions		--	List of actions
	events		--	List of used events
	"""
	def __init__(self, node):
		self.node = node
		self.name = node.get('name')
		utils.check_name(self.name)
		self.description = None
		
		self.id = None
		self.extends = None
		self.abstract = True
		
		self.__flattened = None
		
		self.actions = utils.SingleAssignDictionary("action")
		self.events = EventContainer()
	
	def evaluate(self, tree):
		""" Load values form the XML node. """
		if self.node is None:
			return
		
		self.description = xml_utils.get_description(self.node)
		self.id = xml_utils.get_identifier(self.node)
		
		extends = self.node.get('extends')
		if extends is None:
			self.extends = None
		else:
			try:
				self.extends = tree.components[extends]
			except KeyError:
				raise ParserException("Found unknown super component '%s' in component '%s'!" % (extends, self.name))
		
		# parse functions and events
		self.__parse_actions(tree)
		self.__parse_events(tree)
		
		self.node = None
	
	def __parse_actions(self, tree):
		node = self.node.find('actions')
		if node == None:
			return
		
		for subnode in node.findall('action'):
			element = action.Action(subnode, tree)
			self.actions[element.name] = element
	
	def __parse_events(self, tree):
		event_node = self.node.find('events')
		if event_node == None:
			return
		
		nodes = event_node.find('publish')
		if nodes is not None:
			self.__parse_event(nodes, self.events.publish, tree)
		
		nodes = event_node.find('subscribe')
		if nodes is not None:
			self.__parse_event(nodes, self.events.subscribe, tree)
	
	def __parse_event(self, node, list, tree):
		for node in node.findall('event'):
			event = node.get('name')
			try:
				list[event] = tree.events[event]
			except KeyError:
				raise ParserException("Found unknown event '%s' in component '%s'!" % (event, self.name))
	
	def flattened(self):
		""" Access the version with the flattened hierarchy
		
		In this version the information about the inheritance relations will
		be lost. But the normal version will still contain them.
		
		During the expanding of all super components the value of
		self.__flattened will be used for loop detection. If the value is -1
		it means that during the flattening of the super components our
		component was reached again => unresolveable cycle in the hierarchy tree!
		"""
		if self.extends is None:
			return self
		elif self.__flattened is not None:
			return self.__flattened
		elif isinstance(self.__flattened, int) and self.__flattened == -1:
			raise ParserException("Loop in the definition of component '%s' detected!" % self.name)
		
		self.__flattened = -1
		top = self.extends.flattened()
		
		flat = copy.copy(self)
		flat.actions = flat.actions.copy()
		flat.events = flat.events.copy()
		
		# update missing values with values form the super component
		flat.description = flat.description or top.description
		flat.id = flat.id or top.id
		flat.extends = None
		
		flat.actions.update(top.actions)
		flat.events.update(top.events)
		
		if flat.abstract == False:
			if flat.id is None:
				raise ParserException("No id defined for the non abstract component '%s'!" % flat.name)
		
		self.__flattened = flat
	
	def __cmp__(self, other):
		return cmp(self.id, other.id) or cmp(self.name, other.name)
	
	def dump(self):
		"""
		Print a nice UML-like box of content of the component
		
		Example:
		/================================================\
		|               Stage Object [ee]                |
		|------------------------------------------------|
		| actions:                                       |
		| +[00] ping()                                   |
		| +[01] reset()                                  |
		| +[03] save_configuration()                     |
		|------------------------------------------------|
		| publish:                                       |
		| +[02] error : Error Payload                    |
		| +[0f] debug_message : char[]                   |
		|------------------------------------------------|
		| subscribe:                                     |
		| +[02] error : Error Payload                    |
		\================================================/
		
		The numbers in the square brackets shows the identifier (in
		hexadecimal). First block shows the actions and the second the events.
		
		"""
		if self.id is None:
			name = "%s" % self.name
		else:
			name = "%s [%02x]" % (self.name, self.id)
		max_length = len(name)
		
		elements = [["actions:"], ["publish:"], ["subscribe:"]]
		for element in elements:
			max_length = max(max_length, len(element[0]))
		
		for i, list in enumerate([self.actions, self.events.publish, self.events.subscribe]):
			for element in list:
				string = "+%s" % element
				max_length = max(max_length, len(string))
				elements[i].append(string)
		
		output  = "/=" + "=" * max_length + "=\\\n"
		
		length = (max_length - len(name)) / 2.0
		pre = " " * int(length)
		post = pre + (" " if (length != int(length)) else "")
		output += "| " + pre + name + post + " |\n"
		
		for element in elements:
			output += "|-" + "-" * max_length + "-|\n"
			for string in element:
				length = max_length - len(string)
				output += "| " + string + " " * length + " |\n"
		
		output += "\\=" + "=" * max_length + "=/\n"
		return output[:-1]
	
	def __str__(self):
		if self.id is None:
			name = "[--] %s" % self.name
		else:
			name = "[%02x] %s" % (self.id, self.name)
		return name
