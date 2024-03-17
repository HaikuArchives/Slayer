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
// this hashtable isn't thread safe

#ifndef _HASHTABLE_H
#define _HASHTABLE_H

#include <SupportKit.h>

// Hashtable that can has only one instance of a key
struct _hash_node {
	int32 key;
	void* item;
	struct _hash_node* next;

	inline _hash_node(int32 _key = -1, void* _item = NULL)
	{
		key = _key;
		item = _item;
		next = NULL;
	}
};

class Hashtable {
  protected:
	int32 array_size;
	int32 items;
	_hash_node** array;

  public:
	Hashtable(int32 size = 50);
	~Hashtable();

	inline void* put(int32 key, void* item)
	{
		_hash_node *node = array[key % array_size], *last = NULL;

		for (; node; last = node, node = node->next)
			;

		node = new _hash_node(key, item);
		if (last)
			last->next = node;
		else
			array[key % array_size] = node;

		items++;

		return item;
	}

	inline void* get(int32 key)
	{
		_hash_node* node = array[key % array_size];
		for (; node; node = node->next)
			if (node->key == key)
				return node->item;

		return NULL;
	}

	inline void* del(int32 key)
	{
		_hash_node *node = array[key % array_size], *last = NULL;
		for (; node; last = node, node = node->next)
			if (node->key == key) {
				if (last)
					last->next = node->next;
				else
					array[key % array_size] = node->next;
				items--;
				void* item = node->item;
				delete node;
				return item;
			}

		return NULL;
	}

	void forEachDo(bool (*)(void* item));
	// not implemented yet
	// void rehash(void);
};

#endif
