/// <reference path="../../../cocos2d-typescript-definitions/cocos2d/cocos2d-lib.d.ts" />
var LAYER_INIT = false;

var chimple = chimple || {};
var HelloWorldLayer = cc.Layer.extend({
    ctor: function () {
        this._super();
        this._name = "StoryLayer";
        this._propsContainer = [];
        this._charactersContainer = [];
        return true;
    },
    init: function () {
        //load cache of icons.png and icons.plist to create panel
        // var cache = cc.spriteFrameCache;
        // cache.addSpriteFrames(res.icon_plist, res.icon_png);
        //create dummy variable to hold Text
        cc.log('eeeee:' + this._sceneText);
        if (chimple.storyConfigurationObject) {
            //backgrounds, characters and pops, texts
            var mainConfigurationItems = Object.getOwnPropertyNames(chimple.storyConfigurationObject);
            //Construct UI
            var pageView = new chimple.PageScroller(cc.p(1800, 0), cc.size(760, 1800), 2, 3, mainConfigurationItems, cc.color.RED, this.configurationChoosed, this, false);
            this.addChild(pageView, 2);
        }
    },

    registerEventListenerForAllChildren: function () {
        if (layer) {
            layer.children.forEach(function (element) {
                if (element._name === 'Scene') {
                    element.children.forEach(function (element) {
                        var listener = cc.EventListener.create({
                            event: cc.EventListener.TOUCH_ONE_BY_ONE,
                            swallowTouches: true,
                            onTouchBegan: function (touch, event) {
                                var target = event.getCurrentTarget();
                                var location = target.convertToNodeSpace(touch.getLocation());
                                var targetSize = target.getContentSize();
                                var targetRectangle = cc.rect(0, 0, targetSize.width, targetSize.
                                    height);
                                if (cc.rectContainsPoint(targetRectangle, location)) {
                                    return true;
                                }
                                return false;
                            },

                            onTouchMoved: function (touch, event) {
                                var target = event.getCurrentTarget();
                                var location = target.parent.convertToNodeSpace(touch.getLocation());
                                event.getCurrentTarget().setPosition(location);
                            }
                        });

                        cc.eventManager.addListener(listener, element);
                    }, this);
                }
            }, this);
        }
    },


    loadSceneFromStorage: function () {
        //check if data exists in localstorage with Key
        var storedSceneString = cc.sys.localStorage.getItem(layer.pageKey);
        if (storedSceneString != null) {
            var storedSceneJSON = JSON.parse(storedSceneString);
            this.putIntoCacheFromLocalStorage(layer.pageKey, storedSceneJSON);
            this.doPostLoadingProcessForScene(this, layer.pageKey, false);
        }
    },

    putIntoCacheFromLocalStorage: function (cacheKey, contents) {
        cc.loader.cache[cacheKey] = contents;
    },

    saveSceneToLocalStorage: function (data) {
        cc.sys.localStorage.setItem(layer.pageKey, data);
    },

    configurationChoosed: function (selectedItem) {
        //get configuration for selected Item
        //create scroll bar at top based on item selected

        var selectedConfig = chimple.storyConfigurationObject[selectedItem.getName()];
        cc.log(selectedItem.getName());
        if (selectedConfig != null && selectedItem.getName() != "texts") {
            this.constructTabBar(selectedConfig.categories);
        } else {
            //show text editor
            this.addTextToScene();
        }
    },
    addTextToScene: function () {
        this._sceneText = "how are you today?";
        var textEditScene = new TextEditScene(this._sceneText);
        cc.director.pushScene(textEditScene);
    },

    itemSelectedInConfiguration: function (selectedItem) {
        cc.log('itemSelectedInConfiguration:' + selectedItem);
        this.destoryTabBar();
        this.process(selectedItem);

    },


    process: function (selectedItem) {
        if (selectedItem.dataType === 'png' && selectedItem._pngFileToLoad != null) {
            //process image - create cc.sprite node
            this.loadImageAddToNode(selectedItem);
        } else if (selectedItem.dataType === 'json' && selectedItem._jsonFileToLoad != null) {
            this.loadJsonFile(selectedItem);
        }
    },

    loadImageAddToNode: function (selectedItem) {
        //load image if only not already in cache
        var imageToLoad = selectedItem._pngFileToLoad;
        this.showLoadingScene(imageToLoad, this.doPostLoadingProcessForImage, this, imageToLoad);
    },

    doPostLoadingProcessForImage: function (context, imageToLoad) {
        var sprite = new cc.Sprite(imageToLoad);
        //context.addChild(sprite, 1);
        sprite.setPosition(cc.director.getWinSize().width / 2, cc.director.getWinSize().height / 2);
        sprite.setScale(1);
        
        
        var loadedImageObject = this.constructJSONFromCCSprite(sprite);

        var storedSceneString = cc.sys.localStorage.getItem(layer.pageKey);
        if (storedSceneString != null) {
            var storedSceneJSON = JSON.parse(storedSceneString);
            if(storedSceneJSON) {
                storedSceneJSON.Content.Content.ObjectData.Children.push(loadedImageObject);
                this.saveSceneToLocalStorage(JSON.stringify(storedSceneJSON));
            }
        }
        else {
            this._propsContainer.push(loadedImageObject);
        }


        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                var target = event.getCurrentTarget();
                var location = target.convertToNodeSpace(touch.getLocation());
                var targetSize = target.getContentSize();
                var targetRectangle = cc.rect(0, 0, targetSize.width, targetSize.
                    height);
                if (cc.rectContainsPoint(targetRectangle, location)) {
                    return true;
                }
                return false;
            },

            onTouchMoved: function (touch, event) {
                var target = event.getCurrentTarget();
                var location = target.parent.convertToNodeSpace(touch.getLocation());
                event.getCurrentTarget().setPosition(location);
            }
        });

        cc.eventManager.addListener(listener, sprite);

    },


    constructJSONFromTextSprite: function (sprite) {
        var object = Object.create(Object.prototype);
        object.FontSize = "12";
        object.LabelText = "How are you??";
        object.PlaceHolderText = "";
        object.MaxLengthEnable = true;
        object.MaxLengthText = 50;
        object.AnchorPoint = {
            "ScaleX": sprite._anchorPoint.x,
            "ScaleY": sprite._anchorPoint.y
        };

        object.Position = {
            "X": sprite._position.x,
            "Y": sprite._position.y
        };

        object.RotationSkewX = sprite._rotationX;
        object.RotationSkewY = sprite._rotationY;
        object.Scale = {
            "ScaleX": sprite._scaleX,
            "ScaleY": sprite._scaleY
        };
        object.CColor = {
            "R": sprite.color.r,
            "G": sprite.color.g,
            "B": sprite.color.b,
            "A": sprite.color.a
        };
        object.IconVisible = false;
        object.Size = {
            "X": sprite._rect.width,
            "Y": sprite._rect.height
        };
        object.Alpha = sprite._alpha;
        object.Tag = sprite.tag;
        if (sprite.getName().indexOf("%%") === -1) {
            sprite.setName(sprite.getName() + "%%" + this.generateUUID());
        }

        object.Name = sprite.getName();
        object.ctype = "TextFieldObjectData";

        if (sprite.getComponent('ComExtensionData') && sprite.getComponent('ComExtensionData')._customProperty != null) {
            object.UserData = sprite.getComponent('ComExtensionData')._customProperty;
        };
        return object;
    },


    constructJSONFromCCSprite: function (sprite) {

        var object = Object.create(Object.prototype);
        object.FlipX = sprite._flippedX;
        object.FlipY = sprite._flippedY;
        object.FileData = {};
        object.FileData.Type = "Normal";
        if (sprite.getTexture().url != null) {
            var path = sprite.getTexture().url.replace("res/", "");
            object.FileData.Path = path;
        }
        object.FileData.Plist = "";

        object.BlendFunc = {
            "Src": sprite._blendFunc.src,
            "Dst": sprite._blendFunc.dst
        };


        object.AnchorPoint = {
            "ScaleX": sprite._anchorPoint.x,
            "ScaleY": sprite._anchorPoint.y
        };

        object.Position = {
            "X": sprite._position.x,
            "Y": sprite._position.y
        };

        object.RotationSkewX = sprite._rotationX;
        object.RotationSkewY = sprite._rotationY;
        object.Scale = {
            "ScaleX": sprite._scaleX,
            "ScaleY": sprite._scaleY
        };
        object.CColor = {
            "R": sprite.color.r,
            "G": sprite.color.g,
            "B": sprite.color.b,
            "A": sprite.color.a
        };
        object.IconVisible = false;
        object.Size = {
            "X": sprite._rect.width,
            "Y": sprite._rect.height
        };
        object.Alpha = sprite._alpha;
        object.Tag = sprite.tag;
        if (sprite.getName().indexOf("%%") === -1) {
            sprite.setName(sprite.getName() + "%%" + this.generateUUID());
        }

        object.Name = sprite.getName();
        object.ctype = "SpriteObjectData";

        if (sprite.getComponent('ComExtensionData') && sprite.getComponent('ComExtensionData')._customProperty != null) {
            object.UserData = sprite.getComponent('ComExtensionData')._customProperty;
        };
        return object;
    },

    loadJsonFile: function (selectedItem) {
        //load json file in new window
        //could be following
        //SceneNode - mainScene
        //characterNode 
        //text - no png or json
        //png - sprite image  
        var type = selectedItem._configurationType;
        var fileToLoad = selectedItem._jsonFileToLoad;
        switch (type) {
            case "character":
                this.addCharacterToScene(fileToLoad);
                break;
            case "scene":
                this.createSceneFromFile(fileToLoad);
                break;
        }

    },

    doPostLoadingProcessForScene: function (context, fileToLoad, shouldSaveToLocalStorage) {
        context._constructedScene = ccs.load(fileToLoad);
        if (context._constructedScene != null) {
            context.addChild(context._constructedScene.node, 0);
            context._constructedScene.node.children.forEach(function (element) {
                var listener = cc.EventListener.create({
                    event: cc.EventListener.TOUCH_ONE_BY_ONE,
                    swallowTouches: true,
                    onTouchBegan: function (touch, event) {
                        var target = event.getCurrentTarget();
                        var location = target.convertToNodeSpace(touch.getLocation());
                        var targetSize = target.getContentSize();
                        var targetRectangle = cc.rect(0, 0, targetSize.width, targetSize.
                            height);
                        if (cc.rectContainsPoint(targetRectangle, location)) {
                            return true;
                        }
                        return false;
                    },

                    onTouchMoved: function (touch, event) {
                        var target = event.getCurrentTarget();
                        var location = target.parent.convertToNodeSpace(touch.getLocation());
                        event.getCurrentTarget().setPosition(location);
                    }
                });

                cc.eventManager.addListener(listener, element);

            }, context);

            //parse JSON and store in local storage
            if (shouldSaveToLocalStorage) {
                context.parseScene(context, fileToLoad);
            }
        }
    },

    parseScene: function (context, fileToLoad) {
        cc.log('got file:' + fileToLoad);
        var resourcePath = fileToLoad.substring(0, fileToLoad.lastIndexOf("/") + 1);
        cc.log('resourcePath:' + resourcePath);
        cc.loader.loadJson(fileToLoad, function (error, data) {
            cc.log('data:' + data);
            if (data != null) {
                //context.updateNodesNameToUniqueName(data, resourcePath);

                if (context._propsContainer != null && context._propsContainer.length > 0) {
                    context._propsContainer.forEach(function (propObject) {
                        data.Content.Content.ObjectData.Children.push(propObject);
                    }, context);
                }
                context._propsContainer = [];
                context.saveSceneToLocalStorage(JSON.stringify(data));
            }
        });
    },

    generateUUID: function () {
        var d = new Date().getTime();
        var uuid = 'xxxxxxxx-xxxx-4xxx-yxxx-xxxxxxxxxxxx'.replace(/[xy]/g, function (c) {
            var r = (d + Math.random() * 16) % 16 | 0;
            d = Math.floor(d / 16);
            return (c == 'x' ? r : (r & 0x3 | 0x8)).toString(16);
        });
        return uuid;
    },


    updateNodesNameToUniqueName: function (data, resourcePath) {
        if (data && data.Content && data.Content.Content) {
            var objects = data.Content.Content.ObjectData;
            if (objects) {
                objects.Children.forEach(function (element) {
                    if (element.Name.indexOf("%%") === -1) {
                        element.Name = element.Name + "%%" + this.generateUUID();
                    }

                    //element.FileData.Path = resourcePath + element.FileData.Path;
                }, this);
            }
        }
    },

    createSceneFromFile: function (fileToLoad) {
        if (this._mainScene != null) {
            this._mainScene.node.removeFromParent(true);
        }
        this.showLoadingScene(fileToLoad, this.doPostLoadingProcessForScene, this, fileToLoad, true);
    },


    //later create custom loading screen
    showLoadingScene: function (fileToLoad, doPostLoadingProcessFunction, context, args, shouldSaveToLocalStorage) {
        var loaderScene = cc.LoaderScene;
        cc.director.pushScene(loaderScene);
        var dynamicResources = [fileToLoad];
        loaderScene.preload(dynamicResources, function () {
            cc.director.popScene(loaderScene);
            doPostLoadingProcessFunction.call(context, context, args, shouldSaveToLocalStorage);
        }, this);
    },

    
    
    parseCharacter: function (fileToLoad, load) {
        cc.log('got file:' + fileToLoad);        
        var resourcePath = fileToLoad.replace("res/", "");
        cc.log('resourcePath:' + resourcePath);
        cc.log('skeleton:' + load.node);
        var skeletonObject = this.constructJSONFromCharacter(load.node, resourcePath);
        // context.saveCharacterToLocalStorage(JSON.stringify(skeletonObject));
        cc.log('JSON.stringify(skeletonObject):' +JSON.stringify(skeletonObject));
        var storedSceneString = cc.sys.localStorage.getItem(layer.pageKey);
        if (storedSceneString != null) {
            var storedSceneJSON = JSON.parse(storedSceneString);
            if(storedSceneJSON) {
                storedSceneJSON.Content.Content.ObjectData.Children.push(skeletonObject);
                this.saveSceneToLocalStorage(JSON.stringify(storedSceneJSON));
            }
        }
        
        // cc.loader.loadJson(fileToLoad, function (error, data) {
        //     cc.log('data:' + data);
        //     // context.updateCharacterToUniqueName(data, resourcePath);
        //     context._charactersContainer.push(data);
        //     cc.log('JSON String:' + JSON.stringify(data));
        //     context.saveCharacterToLocalStorage(data);
        // });
    },


constructJSONFromCharacter: function(skeleton, resourcePath) 
    {        
        var object = Object.create(Object.prototype);
        object.FileData = {};
        object.FileData.Type = "Normal";
        object.FileData.Path = resourcePath;
        object.FileData.Plist = "";
        
        object.InnerActionSpeed =  1.0;

        object.AnchorPoint = {
            "ScaleX": skeleton._anchorPoint.x,
            "ScaleY": skeleton._anchorPoint.y
        };

        object.Position = {
            "X": skeleton._position.x,
            "Y": skeleton._position.y
        };

        object.RotationSkewX = skeleton._rotationX;
        object.RotationSkewY = skeleton._rotationY;
        object.Scale = {
            "ScaleX": skeleton._scaleX,
            "ScaleY": skeleton._scaleY
        };
        object.CColor = {
            "R": skeleton.color.r,
            "G": skeleton.color.g,
            "B": skeleton.color.b,
            "A": skeleton.color.a
        };
        object.tag = skeleton.tag;
        object.Size = {
            "X": skeleton.width,
            "Y": skeleton.height
        };

        object.Name = skeleton._name;
        object.ctype = "ProjectNodeObjectData";

        return object;                      
    },
    
    updateCharacterToUniqueName: function (data, resourcePath) {
        if (data && data.Content && data.Content.Content) {
            var objects = data.Content.Content.ObjectData;
            if (objects) {
                if (objects.Name.indexOf("%%") === -1) {
                    objects.Name = objects.Name + "%%" + this.generateUUID();
                }
            }
            //                    element.FileData.Path = resourcePath + element.FileData.Path;

        }
    },


    saveCharacterToLocalStorage: function (data) {
        var charKey = layer.pageKey + "_" + data.Content.Content.ObjectData.Name;
        cc.sys.localStorage.setItem(charKey, JSON.stringify(data));
    },

    addCharacterToScene: function (fileToLoad) {
        var load = ccs.load(fileToLoad);
        
        load.node.setPosition(900, 900);
        var listener = cc.EventListener.create({
            event: cc.EventListener.TOUCH_ONE_BY_ONE,
            swallowTouches: true,
            onTouchBegan: function (touch, event) {
                var target = event.getCurrentTarget();
                var location = target.convertToNodeSpace(touch.getLocation());
                var boundingBox = target.getBoundingBoxToWorld();
                var targetSize = cc.size(boundingBox.width, boundingBox.height);
                var targetRectangle = cc.rect(0, 0, targetSize.width, targetSize.height);
                if (cc.rectContainsPoint(targetRectangle, location)) {
                    return true;
                }
                return false;
            },
            onTouchMoved: function (touch, event) {
                var target = event.getCurrentTarget();
                target.setPosition(target.parent.convertToNodeSpace(touch.getLocation()));
                target._renderCmd._dirtyFlag = 1;
            }
        });
        cc.eventManager.addListener(listener, load.node);

        this.addChild(load.node);
        this.parseCharacter(fileToLoad, load);
        // load.node.runAction(load.action);
        // load.action.play('nod', false);       
    },



    constructTabBar: function (configuration) {
        this._tabBar = new chimple.TabPanel(cc.p(0, 0), cc.size(1800, 1800), 2, 2, configuration, this.itemSelectedInConfiguration, this);
        this.addChild(this._tabBar);
    },

    destoryTabBar: function () {
        this._tabBar.removeFromParent(true);
    },

    saveToFile: function () {
        cc.sys.localStorage;
    }
});

var HelloWorldScene = cc.Scene.extend({
    onEnter: function () {
        this._super();
        if (LAYER_INIT === false) {
            LAYER_INIT = true;
            layer = new HelloWorldLayer();
            this.addChild(layer);
            layer.init();
        }

        layer.registerEventListenerForAllChildren();
        layer.pageKey = "res/chimple.page1.scene.json";
        layer.loadSceneFromStorage();

    }
}
);