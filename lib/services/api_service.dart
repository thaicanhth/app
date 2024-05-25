// import 'dart:convert';
// import 'package:http/http.dart' as http;
// import '../models/device_model.dart';
//
// class ApiService {
//   final String baseUrl = "http://10.0.2.2:3000";
//
//   Future<List<Device>> fetchDevices() async {
//     final response = await http.get(Uri.parse(baseUrl));
//     if (response.statusCode == 200) {
//       List jsonResponse = json.decode(response.body)['data'];
//       return jsonResponse.map((device) => Device.fromJson(device)).toList();
//     } else {
//       throw Exception('Failed to load devices');
//     }
//   }
//
//   Future<Device> addDevice(Device device) async {
//     final response = await http.post(
//       Uri.parse(baseUrl),
//       headers: {"Content-Type": "application/json"},
//       body: jsonEncode({
//         'namecategory': device.namecategory,
//         'colorcategory': device.colorcategory,
//         'category': {
//           'name': device.category.name,
//           'topic': device.category.topic,
//           'status': device.category.status,
//           'color': device.category.color,
//           'voice': device.category.voice,
//           'notification': device.category.notification,
//           'time': device.category.time,
//           'icon': device.category.icon,
//         },
//       }),
//     );
//
//     if (response.statusCode == 201) {
//       return Device.fromJson(json.decode(response.body));
//     } else {
//       throw Exception('Failed to add device: ${response.statusCode}');
//     }
//   }
//
//   Future<void> toggleDeviceStatus(String id, bool status) async {
//     final response = await http.put(
//       Uri.parse('$baseUrl/$id'),
//       headers: {"Content-Type": "application/json"},
//       body: jsonEncode({'category': {'status': status}}),
//     );
//
//     if (response.statusCode != 200) {
//       throw Exception('Failed to update device');
//     }
//   }
// }

