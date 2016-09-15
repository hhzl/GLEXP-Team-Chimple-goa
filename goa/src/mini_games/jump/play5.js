
var xc = xc || {};

xc.play5Layer = cc.Layer.extend( {
   group : [], 
   ballref:[],
   square:[],
   word:[],
   wordObj : [],
   stepRef:[],
   list:[],
   dict :[],
   dict5:[],
   self : null,
   index:0,
   wrongCount:0,
   correct: 0,
   flag:true,
   score:0,
   scoreLabel : null,
   _dir_Flag: true,

    ctor:function () {
        this._super();
        

        this.size = cc.winSize;
        cc.spriteFrameCache.addSpriteFrames(xc.playLayer.res.jump_plist);

         this.char = ccs.load(xc.playLayer.res.char,xc.path);
        this.char.node.setPosition(cc.p(this.size.width - 2450,this.size.height-950));
        this.addChild(this.char.node,1);

        this.bg = ccs.load(xc.playLayer.res.jump_game, xc.path);
        this.addChild(this.bg.node);
        // var child = this.bg.node.getChildren(); 
        //  for(var i=0; i < child.length ;i++)
        //  {
        //     var name = child[i].getName();
        //     cc.log("%s", name);
        //  }
    
//cc.log(cc.FileUtils.getInstance().getStringFromFile(xc.playLayer.res.jump_plist));

       
        
        
        var ball1 = this.bg.node.getChildByName("ball_34");
        ball1.id = "Ball1";
        this.ballref.push(ball1.getPosition());
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , ball1); 

        var ball2 = this.bg.node.getChildByName("ball_35");
        ball2.id = "Ball2";
        this.ballref.push(ball2.getPosition());
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , ball2); 

        var ball3 = this.bg.node.getChildByName("ball_36");
        ball3.id = "Ball3";
        this.ballref.push(ball3.getPosition());
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , ball3); 

        var ball4 = this.bg.node.getChildByName("ball_37");
        ball4.id = "Ball4";
        this.ballref.push(ball4.getPosition());
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , ball4); 

        var ball5 = this.bg.node.getChildByName("ball_38");
        ball5.id = "Ball5";
        this.ballref.push(ball5.getPosition());
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , ball5); 

        var ball6 = this.bg.node.getChildByName("ball_44");
        ball6.id = "Ball6";
        this.ballref.push(ball6.getPosition());
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , ball6); 

        var wrong = this.bg.node.getChildByName("cross_button_33");
        wrong.id="Wrong";
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , wrong); 

        var tick = this.bg.node.getChildByName("tick_button_31");
        tick.id="Tick";
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , tick); 

        var hint = this.bg.node.getChildByName("hint_button_32");
        hint.id="Hint";
        cc.eventManager.addListener(cc.EventListener.create(  
        {event: cc.EventListener.TOUCH_ONE_BY_ONE,
         swallowTouches:true,  
         onTouchBegan: this.onTouchBegan,}) , hint); 


        var square1 =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/box.png"));
        square1.setPosition(cc.p(this.ballref[0].x+140,this.ballref[1].y+260));
        this.addChild(square1,1);
        this.square.push(square1.getPosition());

        var square2 =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/box.png"));
        square2.setPosition(cc.p(this.ballref[1].x+140,this.ballref[2].y+260));
        this.addChild(square2,1);
        this.square.push(square2.getPosition());

        var square3 =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/box.png"));
        square3.setPosition(cc.p(this.ballref[2].x+140,this.ballref[3].y+260));
        this.addChild(square3,1);
        this.square.push(square3.getPosition());

        var square4 =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/box.png"));
        square4.setPosition(cc.p(this.ballref[3].x+140,this.ballref[4].y+260));
        this.addChild(square4,1);
        this.square.push(square4.getPosition());

        var square5 =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/box.png"));
        square5.setPosition(cc.p(this.ballref[4].x+140,this.ballref[4].y+260));
        this.addChild(square5,1);
        this.square.push(square5.getPosition());

        scoreLabel = new cc.LabelTTF(''+this.score,'Arial', 100 );
        scoreLabel.x =cc.winSize.width - 1380;
        scoreLabel.y =cc.winSize.height - 90;
        scoreLabel.setColor(cc.color(0,0,0)); 
        this.addChild(scoreLabel,0); 

      //  this.stepPosition =[{x:2000,y:900},{x:1200,y:600},{x:400,y:300},{x:1200,y:0},{x:2000,y:-300}];
        var step_width = this.size.width /4;
        
    //    for(var i=0; i<5; i++)
    //    {
     //   var step =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/step.png"));    
     //   step.setPosition(cc.p(this.size.width - this.stepPosition[i].x,this.size.height-this.stepPosition[i].y));
     //   this.stepRef.push(step);
     //   step.setName("step");
       // this.addChild(step,1);
   //     }



        this.stepRight =[900 ,1200 , 1500];

        for(var i=1; i< 4; i++)
        {
        var step =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/step.png"));    
        step.setPosition(cc.p(step_width * i, this.stepRight[i-1]));
        this.stepRef.push(step);
        step.setName("step");
        this.addChild(step,1);
        }

       this.stepLeft =[2100,1800];
        for(var i=2; i>0 ; i--)
        {
        var step =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/step.png"));    
        step.setPosition(cc.p(step_width * (i), this.stepLeft[i-1]));
        this.stepRef.push(step);
        step.setName("step");
        this.addChild(step,1);
        }



        this.consonants = ['B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','V','W','X','Y','Z'];
        this.vowels = ['A','E','I','O','U'];

        this.createLevel();
         this.scheduleUpdate();
        self=this;
    },

     update: function(dt){
         if(this.bg.node!= null){

         var child = this.getChildByName("step");
        if(cc.rectIntersectsRect(child.getBoundingBox() , this.bg.node.getChildByName("Panel_1").getChildByName("Panel_5").getBoundingBox()))
         {
             cc.log("done");
             this.removeChild(child);
        
        }
        

         }
     },
     createLevel : function(){

      group =  this.generateRandomLetters(6,this.array);
      for( var i=0; i<6 ;i++){
      var alpha = cc.LabelTTF.create(group[i], "res/fonts/Marker Felt.ttf", 130);
      this.addChild(alpha);
      alpha.setPosition(cc.p(this.ballref[i].x,this.ballref[i].y));
      alpha.setAnchorPoint(0.5,0.5);
      alpha.setColor(cc.color(0,0,0));
        //  cc.log("alpha =",group[i]);
        }

       cc.log(group);
      this.string = '';
        for( var i=0; i< group.length; i++)
        {
            this.string += group[i];
        }
      dict =  xc.WordUtil.getValidCombinations(this.string.toLowerCase());
  // dict = ["abc","dsa","cba","bda","hsf"];
        for(var i=0; i < dict.length ;i++)
         {   if(dict[i].length == 5)
             {
                this.dict5.push(dict[i]); 
             }
         }
        cc.log(this.dict5);
     },


     charMove : function()
     {

        var jump = new cc.jumpBy(1,cc.p(500,160),150,1);
        this.char.node.runAction(jump);

        var animation = ccs.load(xc.playLayer.res.char,xc.path);
       this.char.node.runAction(animation.action);
       animation.action.play("jumping",false);

     },
    charjump: function()
    {
        // if (this.correct % 2 == 0 && this.correct > 2 && this._dir_Flag){
        //      var jump = new cc.jumpBy(1.5,cc.p(this.size.width /4,300),200,1);
        // } else if (this.correct == 2){
        //     var jump = new cc.jumpBy(1.5,cc.p(this.size.width /4,300),200,1);
        // } else {
        //     var jump = new cc.jumpBy(1.5,cc.p(-this.size.width /4,300),200,1);
        // }
       var x = 1;
        if (this._dir_Flag){
           x = 1;
        }else{
            x = -1;
        }
        var jump = new cc.JumpBy(1.5,cc.p(x * this.size.width /4,300),200,1);
       this.char.node.runAction(cc.sequence( jump, cc.callFunc(this.jumpCallback, this)));//runAction(jump);
       var animation = ccs.load(xc.playLayer.res.char,xc.path);
       this.char.node.runAction(animation.action);
       animation.action.play("jumping",false);
    },
   
   
    jumpCallback :function(){ 
       if (this.correct % 2 == 1 && this.correct >2 ){

           this.stepMove();
            if(this.flag == true)
                       {
                         this.stepRightMove();
                         this.flag = false;
                       }
                      else
                       {
                           this.stepLefttMove();
                           this.flag = true;
                       }
       }
    },
     
     
     
      stepRightMove : function()
    { var step_width = this.size.width /4;
        var stepRight =[1800,2100];
        for(var i=2; i<4 ; i++)
        {
        var step =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/step.png"));    
        step.setPosition(cc.p(step_width * (i),stepRight[i-2]));
        this.stepRef.push(step);
        step.setName("step");
        this.addChild(step,1);
        }


    },

       stepLefttMove : function()
    { var step_width = this.size.width /4;
        var stepLeft =[2100,1800];
        for(var i=2; i>0 ; i--)
        {
        var step =new cc.Sprite(cc.spriteFrameCache.getSpriteFrame("jump_on_words/step.png"));    
        step.setPosition(cc.p(step_width * (i),stepLeft[i-1]));
        this.stepRef.push(step);
        step.setName("step");
        this.addChild(step,1);
        }

    }, 


      stepMove : function()
      {
            this._dir_Flag = (!this._dir_Flag);
    for(var i=0; i<this.stepRef.length; i++)
     {
        var moveBy = new cc.MoveBy(1, cc.p(0,-600));
        this.stepRef[i].runAction(moveBy);
    //     if(cc.rectIntersectsRect(this.getChildByName("step").getBoundingBox() , this.bg.node.getChildByName("Panel_1").getChildByName("Panel_5").getBoundingBox()))
    //  {
    //     cc.log("done");
        
    //  }
           
     }
     var moveBy = new cc.MoveBy(1, cc.p(0,-600));
     this.char.node.runAction(moveBy);
      },
  
  
  
    verify : function (word)
    {
        self.correct++;
             var words = '';
     for( var i=0 ; i< word.length ; i++)  {
        
        words += word[i];
        
     }  
     cc.log("done= %s" , words);
     for(var i=0 ; i< this.dict5.length ; i++)
     {   
       if(this.dict5[i].indexOf(words.toLowerCase()) != -1)
        {
         cc.log("oooo");
         this.jumping();
          cc.log( this.dict5.splice( i ,1));

        }    
     else
        {
         cc.log("aaaa");
         
        }     
     } 
    for ( var i=0 ; i < 5 ; i++)
    {
        this.remove(this.index);
    }
    },

   remove : function(index)
  {
        this.removeChild(this.wordObj[(this.index) - 1]);
        this.wordObj.pop(this.wordObj[(this.index) - 1]);
        this.word.pop(this.word[(this.index) - 1]);
        this.index--;  
        if(this.wordObj.length == 0)
        {
            this.index = 0;   
            cc.eventManager.removeListener(this.wordObj[(this.index) - 1]);
        }       

 },
   
    jumping : function()
    {
        this.incrementScore();
        if(self.correct >= 2)
        {
        self.charjump();
        }else {
        self.charMove();
        }
    },
   
   

     help : function ()
     {
         cc.log("hiint");
        this.decrementScore();
        this.random = Math.floor(Math.random()*(this.dict5.length-1));  
        var nameLabel = new cc.LabelTTF(this.dict5[this.random].toUpperCase(),"res/fonts/Marker Felt.ttf", 100 );
		nameLabel.setAnchorPoint(0.5,0.5); 
        nameLabel.x = cc.winSize.width - 1400;
        nameLabel.y = cc.winSize.height - 1200; 
        nameLabel.setColor(cc.color(250,250,250));
        this.addChild(nameLabel, 1);
        this.scheduleOnce(function(){
           this.removeChild(nameLabel);
        },2);

     },

      generateRandomLetters : function(count,array){
      var vow = ['A','E','I','O','U'];
      var con = ['B','C','D','F','G','H','J','K','L','M','N','P','Q','R','S','T','V','W','X','Y','Z'];
      var array = [];
      for(var i=0;i<2;i++)
      {
      this.randomIndex = Math.floor(Math.random()*(vow.length-1));// Math.floor(Math.random() * 5-i);
    // console.log("val =" +this.randomIndex );
      array.push(vow[this.randomIndex]);  
      vow.splice(this.randomIndex,1); 
      }
      for(var i=0;i<count-2;i++)
      {
      this.randomIndex1 = Math.floor(Math.random()*(con.length-1));//Math.floor(Math.random() * 21-i);  
    //  console.log("val =" +this.randomIndex1 );
      array.push(con[this.randomIndex1]);
      con.splice(this.randomIndex1,1);
         
     }  
     return array;
    },
   
     incrementScore : function()
  {
    this.score += 2;
    scoreLabel.setString(""+ this.score);
  
  },
  
   decrementScore : function()
   {
       this.score -= 1;
       scoreLabel.setString(""+ this.score);
   },

   
   
    onTouchBegan: function(touch, event){
         var target = event.getCurrentTarget();
         var location = target.convertToNodeSpace(touch.getLocation());
         var targetSize = target.getContentSize();
         var targetRectangle = cc.rect(0, 0, targetSize.width, targetSize.height);
         if (cc.rectContainsPoint(targetRectangle, location))
                   {
                       if(target.id == "Ball1" && self.word.length < 5){ 
                        var letter = new cc.LabelTTF (group[0], "res/fonts/Marker Felt.ttf", 130);
                        self.addChild(letter,2);
                        letter.setPosition(cc.p(self.square[self.index].x,self.square[self.index].y));
                        letter.setAnchorPoint(0.5,0.5);
                        letter.setColor(cc.color(0,0,0));
                        self.index++;
                        self.word.push(group[0]);
                        self.wordObj.push(letter);
                        cc.log("hello =", self.word); 
                       }
                        if(target.id == "Ball2"&& self.word.length < 5){ 
                         var letter =  new cc.LabelTTF(group[1], "res/fonts/Marker Felt.ttf", 130);
                        self.addChild(letter,2);
                        letter.setPosition(cc.p(self.square[self.index].x,self.square[self.index].y));
                        letter.setAnchorPoint(0.5,0.5);
                        letter.setColor(cc.color(0,0,0));
                        self.index++;
                        self.word.push(group[1]);
                        self.wordObj.push(letter);
                          
                       }
                        if(target.id == "Ball3"&& self.word.length < 5){ 
                         var letter = new cc.LabelTTF(group[2], "res/fonts/Marker Felt.ttf", 130);
                        self.addChild(letter,2);
                        letter.setPosition(cc.p(self.square[self.index].x,self.square[self.index].y));
                        letter.setAnchorPoint(0.5,0.5);
                        letter.setColor(cc.color(0,0,0));
                        self.index++;
                        self.word.push(group[2]);
                        self.wordObj.push(letter);
                          
                       }
                        if(target.id == "Ball4"&& self.word.length < 5){ 
                         var letter = new cc.LabelTTF(group[3], "res/fonts/Marker Felt.ttf", 130);
                        self.addChild(letter,2);
                        letter.setPosition(cc.p(self.square[self.index].x,self.square[self.index].y));
                        letter.setAnchorPoint(0.5,0.5);
                        letter.setColor(cc.color(0,0,0));
                        self.index++;
                        self.word.push(group[3]);
                        self.wordObj.push(letter);
                          
                       }
                        if(target.id == "Ball5"&& self.word.length < 5){ 
                         var letter = new cc.LabelTTF(group[4], "res/fonts/Marker Felt.ttf", 130);
                        self.addChild(letter,2);
                        letter.setPosition(cc.p(self.square[self.index].x,self.square[self.index].y));
                        letter.setAnchorPoint(0.5,0.5);
                        letter.setColor(cc.color(0,0,0));
                        self.index++;
                        self.word.push(group[4]);
                        self.wordObj.push(letter);
                          
                       }
                        if(target.id == "Ball6"&& self.word.length < 5){ 
                        var letter = new cc.LabelTTF(group[5], "res/fonts/Marker Felt.ttf", 130);
                        self.addChild(letter,2);
                        letter.setPosition(cc.p(self.square[self.index].x,self.square[self.index].y));
                        letter.setAnchorPoint(0.5,0.5);
                        letter.setColor(cc.color(0,0,0));
                        self.index++;
                        self.word.push(group[5]);
                        self.wordObj.push(letter);
                     
                       }
                        if(target.id == "Wrong"){ 
                            cc.log("wrong");
                          //  cc.log("got = ",self.word[(self.index) - 1]);
                        
                       self.remove(self.index);
                     /*  self.stepMove();
                       self.wrongCount++;
                       if(self.flag == true)
                       {
                         self.stepRightMove();
                         self.flag = false;
                       }
                      else
                       {
                           self.stepLefttMove();
                           self.flag = true;
                       }*/

                        }
                       if(target.id == "Tick"){ 
                        if( self.word.length == 5)
                        {
                        self.verify(self.word);
                        }
                        cc.log("tick");
                        }    
                       if(target.id == "Hint"){ 
                         self.help();
                         
                       }   
                       
                   }
     return false;
    }              
});

xc.play5Layer.res = {

     jump_main: xc.path +"jump_on_words/jump_on_words_main_menu.json",
    jump_level: xc.path +"jump_on_words/jump_on_words_level_menu.json",
    jump_game: xc.path +"jump_on_words/jump_on_words_game_menu.json",
    char:xc.path +"jump_on_words/character.json",
    jump_plist: xc.path +"jump_on_words/jump_on_words.plist",
    jump_png: xc.path +"jump_on_words/jump_on_words.png",
    dict:xc.path + "english/allwords.json"


}
