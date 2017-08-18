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
 
package org.chimple.bali.service;

import android.app.Service;
import android.content.Intent;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.Messenger;
import android.os.RemoteException;
import android.support.annotation.Nullable;
import android.widget.Toast;

import org.chimple.bali.helper.LessonHelper;
import org.chimple.bali.model.MultipleChoiceQuiz;

public class LessonService extends Service {
    public static final int MULTIPLE_CHOICE_QUIZ = 1;
    public static final int BAG_OF_CHOICES_QUIZ = 2;

    public static final String NUM_QUIZES = "numQuizes";
    public static final String NUM_CHOICES = "numChoices";

    /**
     * Handler of incoming messages from clients.
     */
    class IncomingHandler extends Handler {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MULTIPLE_CHOICE_QUIZ:
                    Bundle obj = (Bundle) msg.obj;
                    int numQuizes = obj.getInt(NUM_QUIZES);
                    int numChoices = obj.getInt(NUM_CHOICES);
//                    MultipleChoiceQuiz mcqs[] = LessonHelper.getMultipleChoiceQuizes(getApplicationContext(), numQuizes, numChoices);
                    Messenger messenger = msg.replyTo;
                    Message reply = Message.obtain(null, LessonService.MULTIPLE_CHOICE_QUIZ, 0, 0);
                    Bundle bundle = new Bundle();
                    bundle.putStringArray("key", new String[]{"Apple", "Boy", "Cat", "Dog"});
                    bundle.putString("help", "Its all right!");
                    reply.obj = bundle;
//                    reply.obj = new MultipleChoiceQuiz("dummy help",
//                            "A",
//                            new String[]{"Apple", "Boy", "Cat", "Dog"},
//                            0);
                    try {
                        messenger.send(reply);
                    } catch (RemoteException e) {
                        e.printStackTrace();
                    }
                    break;
                default:
                    super.handleMessage(msg);
            }
        }
    }

    /**
     * Target we publish for clients to send messages to IncomingHandler.
     */
    final Messenger mMessenger = new Messenger(new IncomingHandler());

    @Override
    public void onCreate() {
        super.onCreate();
    }

    @Nullable
    @Override
    public IBinder onBind(Intent intent) {
        Toast.makeText(getApplicationContext(), "binding", Toast.LENGTH_SHORT).show();
        return mMessenger.getBinder();
    }
}
