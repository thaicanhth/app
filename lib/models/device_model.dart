// lib/models/device.dart
class Device {
  final String id;
  final String namecategory;
  final String colorcategory;
  final Category category;


  Device({
    required this.id,
    required this.namecategory,
    required this.colorcategory,
    required this.category,
  });

  factory Device.fromJson(Map<String, dynamic> json) {
    return Device(
      id: json['_id'],
      namecategory: json['namecategory'],
      colorcategory: json['colorcategory'],
      category: Category.fromJson(json['category']),
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'namecategory': namecategory,
      'colorcategory': colorcategory,
      'category': category.toJson(),
    };
  }
}

class Category {
  final String name;
  final String topic;
  final bool status;
  final String color;
  final String voice;
  final bool notification;
  final String time;
  final String icon;

  Category({
    required this.name,
    required this.topic,
    required this.status,
    required this.color,
    required this.voice,
    required this.notification,
    required this.time,
    required this.icon,
  });

  factory Category.fromJson(Map<String, dynamic> json) {
    return Category(
      name: json['name'],
      topic: json['topic'],
      status: json['status'],
      color: json['color'],
      voice: json['voice'],
      notification: json['notification'],
      time: json['time'],
      icon: json['icon'],
    );
  }

  Map<String, dynamic> toJson() {
    return {
      'name': name,
      'topic': topic,
      'status': status,
      'color': color,
      'voice': voice,
      'notification': notification,
      'time': time,
      'icon': icon,
    };
  }
}
