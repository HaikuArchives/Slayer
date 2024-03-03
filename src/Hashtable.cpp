/**********************************************************************
 * Copyright 1999-2013 Arto Jalkanen
 *
 * This file is part of Slayer.
 *
 * Slayer is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Slayer is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Slayer.  If not, see <http://www.gnu.org/licenses/>
 **/
#include "Hashtable.h"

/*
_hash_node::_hash_node(int32 _key, void *_item)
{
	key = _key;
	item = _item;
	next = NULL;
}
*/

Hashtable::Hashtable(int32 size) {
	array_size = size;
	items = 0;
	array = new _hash_node *[array_size];
	// initialize to zero
	for (size = 0; size < array_size; size++)
		array[size] = NULL;
}

Hashtable::~Hashtable() {
	if (array)
		delete[] array;
}

/*
void *Hashtable::put(int32 key, void *item)
{
	_hash_node *node = array[key % array_size],
		*last = NULL;

	for (; node; last = node, node = node->next);

	node = new _hash_node(key, item);
	if (last) last->next = node;
	else array[key % array_size] = node;

	items++;

	return item;
}

void *Hashtable::get(int32 key)
{
	_hash_node *node = array[key % array_size];
	for (; node; node = node->next)
		if (node->key == key) return node->item;

	return NULL;
}

void *Hashtable::del(int32 key)
{
	_hash_node *node = array[key % array_size],
		*last = NULL;
	for (; node; last = node, node = node->next)
		if (node->key == key) {
			if (last)
				last->next = node->next;
			else
				array[key % array_size] = node->next;
			items--;
			void *item = node->item;
			delete node;
			return item;
		}

	return NULL;
}
*/

void
Hashtable::forEachDo(bool (*func)(void *)) {
	_hash_node *node;
	int32 i;

	for (i = 0; i < array_size; i++)
		for (node = array[i]; node; node = node->next)
			func(node->item);
}
