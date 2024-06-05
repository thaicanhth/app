// lib/pages/add_device_page.dart
import 'package:flutter/material.dart';
import 'package:flutter_colorpicker/flutter_colorpicker.dart';
import 'package:http/http.dart' as http;
import 'dart:convert';

import 'device_model.dart';

class AddDevicePage extends StatefulWidget {
  final Device? device;

  const AddDevicePage({Key? key, this.device}) : super(key: key);

  @override
  _AddDevicePageState createState() => _AddDevicePageState();
}

class _AddDevicePageState extends State<AddDevicePage> {
  final _formKey = GlobalKey<FormState>();
  late String _nameCategory;
  late String _colorCategory;
  late String _name;
  late String _topic;
  late bool _status;
  late String _color;
  late String _voice;
  late bool _notification;
  late String _time;
  late String _icon;

  @override
  void initState() {
    super.initState();
    if (widget.device != null) {
      _nameCategory = widget.device!.namecategory;
      _colorCategory = widget.device!.colorcategory;
      _name = widget.device!.category.name;
      _topic = widget.device!.category.topic;
      _status = widget.device!.category.status;
      _color = widget.device!.category.color;
      _voice = widget.device!.category.voice;
      _notification = widget.device!.category.notification;
      _time = widget.device!.category.time;
      _icon = widget.device!.category.icon;
    } else {
      _nameCategory = '';
      _colorCategory = '';
      _name = '';
      _topic = '';
      _status = false;
      _color = '';
      _voice = '';
      _notification = false;
      _time = '';
      _icon = '';
    }
  }

  Future<void> addOrUpdateDevice() async {
    if (_formKey.currentState!.validate()) {
      _formKey.currentState!.save();

      final url = widget.device == null
          ? 'http://192.168.127.1:3000/devices'
          : 'http://192.168.127.1:3000/device${widget.device!.id}';

      final method = widget.device == null ? 'POST' : 'PUT';

      final response = await http.post(
        Uri.parse(url),
        headers: <String, String>{
          'Content-Type': 'application/json; charset=UTF-8',
        },
        body: jsonEncode({
          'namecategory': _nameCategory,
          'colorcategory': _colorCategory,
          'category': {
            'name': _name,
            'topic': _topic,
            'status': _status,
            'color': _color,
            'voice': _voice,
            'notification': _notification,
            'time': _time,
            'icon': _icon,
          }
        }),
      );

      if (response.statusCode == 201 || response.statusCode == 200) {
        Navigator.pop(context, true);
      } else {
        throw Exception('Failed to add/update device');
      }
    }
  }

  void selectColor() {
    showDialog(
      context: context,
      builder: (BuildContext context) {
        Color pickerColor = Color(0xff443a49);
        return AlertDialog(
          title: Text('Pick a color'),
          content: SingleChildScrollView(
            child: ColorPicker(
              pickerColor: pickerColor,
              onColorChanged: (Color color) {
                setState(() {
                  pickerColor = color;
                  _color = '#${pickerColor.value.toRadixString(16).substring(2)}';
                });
              },
            ),
          ),
          actions: <Widget>[
            ElevatedButton(
              child: Text('Got it'),
              onPressed: () {
                Navigator.of(context).pop();
              },
            ),
          ],
        );
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(title: Text(widget.device == null ? 'Add Device' : 'Edit Device')),
      body: Padding(
        padding: EdgeInsets.all(16.0),
        child: Form(
          key: _formKey,
          child: ListView(
            children: [
              TextFormField(
                initialValue: _nameCategory,
                decoration: InputDecoration(labelText: 'Name Category'),
                validator: (value) => value!.isEmpty ? 'Please enter a name category' : null,
                onSaved: (value) => _nameCategory = value!,
              ),
              TextFormField(
                initialValue: _colorCategory,
                decoration: InputDecoration(labelText: 'Color Category'),
                validator: (value) => value!.isEmpty ? 'Please enter a color category' : null,
                onSaved: (value) => _colorCategory = value!,
              ),
              TextFormField(
                initialValue: _name,
                decoration: InputDecoration(labelText: 'Name'),
                validator: (value) => value!.isEmpty ? 'Please enter a name' : null,
                onSaved: (value) => _name = value!,
              ),
              TextFormField(
                initialValue: _topic,
                decoration: InputDecoration(labelText: 'Topic'),
                validator: (value) => value!.isEmpty ? 'Please enter a topic' : null,
                onSaved: (value) => _topic = value!,
              ),
              SwitchListTile(
                title: Text('Status'),
                value: _status,
                onChanged: (value) {
                  setState(() {
                    _status = value;
                  });
                },
              ),
              ListTile(
                title: Text('Color'),
                subtitle: Text(_color),
                onTap: selectColor,
              ),
              TextFormField(
                initialValue: _voice,
                decoration: InputDecoration(labelText: 'Voice'),
                validator: (value) => value!.isEmpty ? 'Please enter a voice' : null,
                onSaved: (value) => _voice = value!,
              ),
              SwitchListTile(
                title: Text('Notification'),
                value: _notification,
                onChanged: (value) {
                  setState(() {
                    _notification = value;
                  });
                },
              ),
              TextFormField(
                initialValue: _time,
                decoration: InputDecoration(labelText: 'Time'),
                validator: (value) => value!.isEmpty ? 'Please enter a time' : null,
                onSaved: (value) => _time = value!,
              ),
              TextFormField(
                initialValue: _icon,
                decoration: InputDecoration(labelText: 'Icon URL'),
                validator: (value) => value!.isEmpty ? 'Please enter an icon URL' : null,
                onSaved: (value) => _icon = value!,
              ),
              SizedBox(height: 20.0),
              ElevatedButton(
                onPressed: addOrUpdateDevice,

                child: Text(widget.device == null ? 'Add Device' : 'Update Device'),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
