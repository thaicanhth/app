import 'package:app/providers/user_provider.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import '../models/device_model.dart';
import '../services/auth_services.dart';
import '../util/smart_device_box.dart';
import 'drawer.dart';
import 'profile_page.dart';
import 'dart:convert';
import 'package:http/http.dart' as http;

class HomePage extends StatefulWidget {
  const HomePage({super.key});

  @override
  State<HomePage> createState() => _HomePageState();
}

class _HomePageState extends State<HomePage> {
  // List<String> menuItems = [
  //   "All phòng",
  //   "Phòng khách",
  //   "Phòng ngủ",
  //   "Phòng tắm",
  //   "Phòng bếp",
  // ];
  int selectedIndex = 0;

  List<Device> devices = [];

  @override
  void initState() {
    super.initState();
    fetchDevices();
  }

  void signOutUser(BuildContext context) {
    AuthService().signOut(context);
  }

  // padding constants
  final double horizontalPadding = 40;
  final double verticalPadding = 25;

  Future<void> fetchDevices() async {
    final response = await http.get(Uri.parse('http://192.168.1.9:3000/device'));
    if (response.statusCode == 200) {
      final List<dynamic> deviceJson = json.decode(response.body)['data'];
      setState(() {
        devices = deviceJson.map((json) => Device.fromJson(json)).toList();
      });
    } else {
      throw Exception('Failed to load devices');
    }
  }

  Future<void> toggleDeviceStatus(Device device) async {
    final newStatus = !device.category.status;
    final response = await http.put(
      Uri.parse('http://192.168.1.9:3000/device/${device.id}'),
      headers: <String, String>{
        'Content-Type': 'application/json; charset=UTF-8',
      },
      body: jsonEncode({
        'namecategory': device.namecategory,
        'colorcategory': device.colorcategory,
        'category': {
          'name': device.category.name,
          'topic': device.category.topic,
          'status': newStatus,
          'color': device.category.color,
          'voice': device.category.voice,
          'notification': device.category.notification,
          'time': device.category.time,
          'icon': device.category.icon,
        }
      }),
    );

    if (response.statusCode == 200) {
      fetchDevices();
    } else {
      throw Exception('Failed to update device');
    }
  }

  Future<void> deleteDevice(String deviceId) async {
    final response = await http.delete(
      Uri.parse('http://192.168.1.9:3000/device/$deviceId'),
      headers: <String, String>{
        'Content-Type': 'application/json; charset=UTF-8',
      },
    );

    if (response.statusCode == 200) {
      fetchDevices();
    } else {
      throw Exception('Failed to delete device');
    }
  }

  void goToEditDevicePage(Device device) {
    Navigator.pushNamed(
      context,
      '/addDevice',
      arguments: device,
    ).then((value) {
      if (value == true) {
        fetchDevices();
      }
    });
  }

  void goToProfilePage() {
    Navigator.pop(context);
    Navigator.push(
        context,
        MaterialPageRoute(
          builder: (context) => const ProfilePage(),
        ));
  }

  @override
  Widget build(BuildContext context) {
    final user = Provider.of<UserProvider>(context).user;
    return Scaffold(
      backgroundColor: Colors.grey[300],
      appBar: AppBar(
        backgroundColor: Colors.grey[300],
        elevation: 0,
        actions: [
          ElevatedButton(
            onPressed: () => signOutUser(context),
            style: ButtonStyle(
              backgroundColor: MaterialStateProperty.all(Colors.grey[300]),
            ),
            child: const Icon(Icons.logout_outlined, color: Colors.black),
          ),
          Stack(
            children: [
              IconButton(
                onPressed: () {},
                icon: const Icon(
                  Icons.notifications_none,
                  size: 30,
                ),
              ),
              Positioned(
                top: 0,
                right: 6,
                child: Container(
                  padding: const EdgeInsets.all(4),
                  decoration: BoxDecoration(
                    color: Colors.red[500],
                    shape: BoxShape.circle,
                  ),
                  child: const Text(
                    '1',
                    style: TextStyle(fontSize: 10),
                  ),
                ),
              ),
            ],
          )
        ],
      ),
      drawer: MyDrawer(
        onProfileTap: goToProfilePage,
      ),
      body: RefreshIndicator(
        child: SafeArea(
          child: Column(
            crossAxisAlignment: CrossAxisAlignment.start,
            children: [
              Padding(
                padding: EdgeInsets.symmetric(
                  horizontal: horizontalPadding,
                  vertical: verticalPadding,
                ),
              ),
              Padding(
                padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
                child: Column(
                  children: [
                    Row(
                      children: [
                        SizedBox(
                          child: SizedBox(
                            height: 60,
                            width: 60,
                            child: ClipRRect(
                              borderRadius: BorderRadiusDirectional.circular(20),
                              child: Image.asset(
                                'lib/icons/man.jpg',
                                fit: BoxFit.cover,
                              ),
                            ),
                          ),
                        ),
                        const SizedBox(
                          width: 5,
                        ),
                        Column(
                          crossAxisAlignment: CrossAxisAlignment.start,
                          children: [
                            Text(
                              "Hello ${user.name}",
                              style: TextStyle(
                                  fontSize: 20,
                                  color: Colors.grey.shade800,
                                  fontWeight: FontWeight.bold),
                            ),
                            Text(
                              'Welcome To Home',
                              style: TextStyle(
                                  fontSize: 20, color: Colors.grey.shade900),
                            ),
                          ],
                        ),
                      ],
                    )
                  ],
                ),
              ),
              const SizedBox(height: 10),
              Row(
                children: [
                  Expanded(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Padding(
                          padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              Row(
                                children: [
                                  Image.asset(
                                    'lib/icons/temperature.png',
                                    width: 40,
                                    height: 40,
                                  ),
                                ],
                              ),
                              Text(
                                "26°c",
                                style: TextStyle(
                                  fontSize: 16,
                                  fontWeight: FontWeight.w600,
                                  color: Colors.grey[800],
                                ),
                              ),
                              Text(
                                "Temperature",
                                style: TextStyle(
                                    fontSize: 14, color: Colors.grey[800]),
                              )
                            ],
                          ),
                        )
                      ],
                    ),
                  ),
                  Expanded(
                    child: Column(
                      crossAxisAlignment: CrossAxisAlignment.start,
                      children: [
                        Padding(
                          padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
                          child: Column(
                            crossAxisAlignment: CrossAxisAlignment.start,
                            children: [
                              Row(
                                children: [
                                  Image.asset(
                                    'lib/icons/humidity.png',
                                    width: 40,
                                    height: 40,
                                  ),
                                ],
                              ),
                              Text(
                                "59%",
                                style: TextStyle(
                                  fontSize: 16,
                                  fontWeight: FontWeight.w600,
                                  color: Colors.grey[800],
                                ),
                              ),
                              Text(
                                "Humidity",
                                style: TextStyle(
                                    fontSize: 14, color: Colors.grey[800]),
                              )
                            ],
                          ),
                        )
                      ],
                    ),
                  ),
                ],
              ),
              const SizedBox(height: 5),
              const Padding(
                padding: EdgeInsets.symmetric(horizontal: 40.0),
                child: Divider(
                  thickness: 1,
                  color: Color.fromARGB(255, 204, 204, 204),
                ),
              ),
              const SizedBox(height: 5),
              const SizedBox(height: 5),
              // Padding(
              //   padding: const EdgeInsets.all(8.0),
              //   child: LocationMenu(
              //     menuItems: menuItems,
              //     selectedIndex: selectedIndex,
              //     onItemTapped: (index) {
              //       setState(() {
              //         selectedIndex = index;
              //       });
              //     },
              //   ),
              // ),
              const SizedBox(height: 15),
              Padding(
                padding: EdgeInsets.symmetric(horizontal: horizontalPadding),
                child: Text(
                  "${devices.length} Active Devices",
                  style: TextStyle(
                    fontWeight: FontWeight.bold,
                    fontSize: 24,
                    color: Colors.grey.shade800,
                  ),
                ),
              ),
              Expanded(
                child: GridView.builder(
                  itemCount: devices.length,
                  padding: const EdgeInsets.symmetric(horizontal: 15),
                  gridDelegate: const SliverGridDelegateWithFixedCrossAxisCount(
                    crossAxisCount: 2,
                    childAspectRatio: 1 / 1.3,
                  ),
                  itemBuilder: (context, index) {
                    final device = devices[index];
                    return SmartDeviceBox(
                      smartDeviceName: device.category.name,
                      iconPath: int.parse(device.category.icon),
                      powerOn: device.category.status,
                      onChanged: (value) => toggleDeviceStatus(device),
                      onEdit: () => goToEditDevicePage(device),
                      onDelete: () => deleteDevice(device.id),
                    );
                  },
                ),
              )
            ],
          ),
        ),
        onRefresh: () => fetchDevices(),
      ),
      // floatingActionButton: FloatingActionButton(
      //   onPressed: () {
      //     Navigator.pushNamed(context, '/addDevice').then((value) {
      //       if (value == true) {
      //         fetchDevices();
      //       }
      //     });
      //   },
      //   child: Icon(Icons.add),
      // ),
    );
  }
}
