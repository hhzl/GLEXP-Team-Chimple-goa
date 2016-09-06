

var sortitMovableItem = cc.Sprite.extend({
        
        ctor:function(imageFile, transparentSprite, counterLevel1) {
            this._super();
            this.initWithFile(imageFile); 
            var transparentSprite = transparentSprite;

            var overlapped = 0;


var sprite_click = cc.EventListener.create({event: cc.EventListener.TOUCH_ONE_BY_ONE, swallowTouches: true,
  

  onTouchBegan :function(touch, event){

  	var target = event.getCurrentTarget();
    var location = target.convertToNodeSpace(touch.getLocation());
    var targetSize = target.getContentSize();
    var targetRectangle = cc.rect(0,0, target.width, target.height);

     if (cc.rectContainsPoint(targetRectangle, location)){return true;}
      
     return false;

  },

  onTouchMoved : function(touch, event){

    
  	 var target = event.getCurrentTarget();
         
         var location = target.convertToNodeSpace(touch.getLocation());
		target.setPosition(touch.getLocation());  

        var toyRect = target.getBoundingBox();
        var toytRect = transparentSprite.getBoundingBox();
 
        if(cc.rectIntersectsRect(toyRect, toytRect) && target.id == window.counterLevel1){
           
            var x = transparentSprite.getPosition().x;
            var y = transparentSprite.getPosition().y;
            target.setPosition(x, y);

            this.audioEngine = cc.audioEngine;
            this.audioEngine.playEffect(xc.sortitlevel1Layer.res.comedyBubble_mp3);


            overlapped = 1;
            window.counterLevel1++;

            cc.eventManager.removeListener(sprite_click, target);
         

            
        }
           

               
  },

  onTouchEnded : function(touch, event){

  		var target = event.getCurrentTarget();
         
         var location = target.convertToNodeSpace(touch.getLocation());
                 
         
                 var toy = target.getContentSize();
                 var rectToy = cc.rect(0, 0, target.width, target.height);
                 if (cc.rectContainsPoint(rectToy, location) && overlapped==0) { var toy = cc.MoveTo.create(2,cc.p(target.xP,target.yP));
        target.runAction(toy);this.audioEngine = cc.audioEngine;
            this.audioEngine.playEffect(xc.sortitlevel1Layer.res.failure_mp3);
return true;}

  }   

});            
            cc.eventManager.addListener(sprite_click, this);
                       
        }
    });


