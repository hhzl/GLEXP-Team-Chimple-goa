/*
 * Copyright 2017, Team Chimple
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.chimple.bali.db.entity;

import android.arch.persistence.room.ColumnInfo;
import android.arch.persistence.room.Entity;
import android.arch.persistence.room.Ignore;
import android.arch.persistence.room.PrimaryKey;

@Entity
public class Unit {
    @PrimaryKey(autoGenerate = true)
    public Long id;

    public String name;

    public int type;

    public String picture;

    public String sound;

    public String phonemeSound;

    @Ignore
    public Unit(String name, int type, String picture, String sound, String phonemeSound) {
        this.name = name;
        this.type = type;
        this.picture = picture;
        this.sound = sound;
        this.phonemeSound = phonemeSound;
    }

    public Unit() {
    }
}
