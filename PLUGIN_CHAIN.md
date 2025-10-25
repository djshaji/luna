# Luna LV2 Host - Active Plugin Cards Feature

## 🎛️ Plugin Cards Overview

The Active Plugin Cards feature transforms Luna into a professional plugin host with an intuitive two-panel design. Each plugin is displayed as an interactive card with real-time parameter controls, bypass options, and removal capabilities.

## 🎯 Key Features

### **Interactive Plugin Cards**
- **Real-time Parameter Controls**: Each plugin displays as a card with sliders for all control parameters
- **Individual Plugin Management**: Bypass or remove plugins independently
- **Parameter Value Display**: Live parameter values shown next to each control
- **Modern Card Design**: Clean, professional interface with proper spacing and fonts

### **Two-Panel Layout**
- **Left Panel**: Available LV2 Plugins browser (30% of window width)
- **Right Panel**: Active Plugin Cards with controls (70% of window width)
- **No Clutter**: Simplified design focusing on essential functionality

### **Easy Plugin Management**
- **Double-Click Addition**: Double-click any plugin to add it as an interactive card
- **Bypass Control**: 🔊 Active / 🔇 Bypassed toggle for each plugin
- **Individual Removal**: 🗑️ Remove button on each plugin card
- **Clear All**: Single button to remove all active plugins
- **Real-time Feedback**: Status updates for all operations

## 🖱️ User Interface Layout

```
┌────────────────────────────────────────────────────────────────────────┐
│ 🎵 Luna LV2 Host - Digital Audio Workstation          [▶ Start Audio] │
├────────────────────────────────────────────────────────────────────────┤
│                                                                        │
│ 🔌 Available LV2 Plugins          🎛️ Active Plugins    [🗑️ Clear All] │
│ ┌─────────────────────────┐      ┌─────────────────────────────────────┐ │
│ │ • Reverb Plugin         │      │ ┌─ �️ Reverb Plugin ──────────────┐ │ │
│ │ • Compressor            │      │ │ Param 1: ████████░░ 80.5        │ │ │
│ │ • EQ Three Band         │      │ │ Param 2: ████░░░░░░ 40.0   [🔊][🗑️] │ │
│ │ • Chorus                │      │ │ Param 3: ██████████ 100.0       │ │ │
│ │ • Delay                 │      │ │ Param 4: █████░░░░░ 50.0        │ │ │
│ │ • ...                   │      │ └─────────────────────────────────────┘ │ │
│ │                         │      │ ┌─ 🎚️ Compressor ──────────────────┐ │ │
│ │ [Double-click to add    │      │ │ Threshold: ██████░░░░ 60.0       │ │ │
│ │  as interactive card]   │      │ │ Ratio:     ████████░░ 80.0  [🔇][🗑️] │ │
│ │                         │      │ │ Attack:    ███░░░░░░░ 30.0       │ │ │
│ └─────────────────────────┘      │ │ Release:   ███████░░░ 70.0       │ │ │
│                                  │ └─────────────────────────────────────┘ │ │
│                                  │                                       │ │
├────────────────────────────────────────────────────────────────────────┤
│ Status: Added plugin card: Reverb Plugin                              │
└────────────────────────────────────────────────────────────────────────┘
```

## 🔧 How to Use

### **Adding Plugin Cards**
1. **Browse Available Plugins**: View all detected LV2 plugins in the left panel (30% width)
2. **Double-Click to Add**: Double-click any plugin to create an interactive card
3. **Immediate Control**: Each plugin appears as a card with parameter sliders
4. **Visual Feedback**: Status bar confirms plugin card creation

### **Working with Plugin Cards**
- **Parameter Control**: Use sliders to adjust plugin parameters in real-time
- **Value Monitoring**: Current parameter values displayed next to each slider
- **Plugin Bypass**: Toggle 🔊 Active / 🔇 Bypassed state for each plugin
- **Individual Removal**: Click 🗑️ Remove button on any plugin card
- **Bulk Operations**: Use "🗑️ Clear All" button to remove all plugin cards

### **Plugin Card Features**
- **�️ Plugin Name**: Shows the actual LV2 plugin name
- **Parameter Sliders**: Up to 4 primary parameters with real names
- **Control Buttons**: Bypass and Remove buttons for each plugin
- **Modern Design**: Clean cards with proper spacing and fonts

## 📋 Context Menu Options

Right-click on any plugin in the active chain to access:
- **🗑️ Remove Plugin**: Remove the selected plugin from the chain
- **⬆️ Move Up**: Move plugin earlier in the processing chain
- **⬇️ Move Down**: Move plugin later in the processing chain
- **🗑️ Clear Chain**: Remove all plugins from the chain

## 🎛️ Plugin Card Processing

### **Audio Signal Flow**
```
Input → Plugin Card 1 → Plugin Card 2 → Plugin Card 3 → ... → Output
 🎤       🎚️ Controls     🎚️ Controls     🎚️ Controls          �
           ├─ Param 1      ├─ Param 1      ├─ Param 1
           ├─ Param 2      ├─ Param 2      ├─ Param 2  
           ├─ Param 3      ├─ Param 3      ├─ Param 3
           └─ Param 4      └─ Param 4      └─ Param 4
```

### **Processing Features**
1. **Individual Control**: Each plugin card operates independently with its own parameters
2. **Real-time Updates**: Parameter changes take effect immediately during audio processing
3. **Bypass Capability**: Individual plugins can be bypassed without affecting others
4. **Parameter Automation**: Direct slider control for immediate audio manipulation

## 🚀 Technical Features

### **Dynamic Chain Management**
- **Real-time Addition/Removal**: Modify chain while audio is running
- **Memory Management**: Automatic cleanup of plugin instances
- **Error Handling**: Graceful handling of plugin load failures

### **Visual Feedback**
- **Status Updates**: Real-time status bar messages for all chain operations
- **Empty State**: Clear guidance when the chain is empty
- **Chain Validation**: Visual confirmation of successful plugin additions

### **Integration with Audio Engine**
- **Seamless Processing**: Chain integrates with Luna's audio engine
- **Low Latency**: Optimized for real-time audio processing
- **Plugin State**: Maintains plugin parameters and state

## 💡 Tips and Best Practices

### **Chain Organization**
- **Input Effects First**: Place input processing (gates, compressors) early in chain
- **Modulation Effects**: Add chorus, flangers, phasers in the middle
- **Time-based Effects**: Put reverbs and delays toward the end
- **Output Processing**: Use limiters and finalizers as the last plugins

### **Performance Optimization**
- **Monitor CPU Usage**: Keep an eye on system performance with complex chains
- **Plugin Quality**: Use high-quality, well-optimized LV2 plugins
- **Chain Length**: Balance processing quality with system resources

### **Workflow Efficiency**
- **Save Configurations**: Plan to save common plugin chain configurations
- **Quick Access**: Keep frequently used plugins easily accessible
- **Experimentation**: Try different plugin orders for creative effects

## 🔄 Future Enhancements

### **Planned Features**
- **Drag & Drop**: Visual drag-and-drop reordering of plugins in chain
- **Chain Presets**: Save and load common plugin chain configurations
- **Plugin Parameters**: Real-time control of individual plugin parameters
- **Chain Bypass**: Toggle entire chain or individual plugins on/off
- **Visual Meters**: Audio level indicators between chain stages

### **Advanced Capabilities**
- **Parallel Processing**: Support for parallel plugin chains
- **Send/Return**: Auxiliary sends to external effect chains
- **Plugin Automation**: Parameter automation and modulation
- **MIDI Integration**: MIDI control of plugin chain parameters

The Active Plugin Chain feature transforms Luna from a simple plugin host into a powerful audio processing environment, enabling users to create complex, professional-quality audio processing chains with an intuitive, modern interface.