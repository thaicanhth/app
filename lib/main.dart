import 'package:app/pages/home_page.dart';
import 'package:app/pages/next_home.dart';
import 'package:flutter/material.dart';
import 'package:provider/provider.dart';
import 'models/add_device_page.dart';
import 'models/device_model.dart';
import 'providers/user_provider.dart';
import 'services/auth_services.dart';

void main() {
  runApp(
    MultiProvider(
      providers: [
        ChangeNotifierProvider(create: (_) => UserProvider()),
      ],
      child: const MyApp(),
    ),
  );
}

class MyApp extends StatefulWidget {
  const MyApp({super.key});

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  final AuthService authService = AuthService();

  @override
  void initState() {
    super.initState();
    authService.getUserData(context);
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      debugShowCheckedModeBanner: false,
      title: 'Flutter Node Auth',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: Provider.of<UserProvider>(context).user.token.isEmpty ?  const HomePage() :  const HomePage(),
      routes: {
        '/addDevice': (context) => AddDevicePage(
          device: ModalRoute.of(context)?.settings.arguments as Device?,
        ),
      },
    );
  }
}