// import 'package:flutter/material.dart';
// import 'package:provider/provider.dart';
// import '../providers/user_provider.dart';
// import '../services/auth_services.dart';
//
// class HomeScreen extends StatelessWidget {
//   const HomeScreen({super.key});
//
//   void signOutUser(BuildContext context) {
//     AuthService().signOut(context);
//   }
//
//   @override
//   Widget build(BuildContext context) {
//     final user = Provider.of<UserProvider>(context).user;
//
//     return Scaffold(
//       body: Column(
//         mainAxisAlignment: MainAxisAlignment.center,
//         children: [
//           Text(user.id),
//           Text(user.email),
//           Text(user.name),
//           ElevatedButton(
//             onPressed: () => signOutUser(context),
//             style: ButtonStyle(
//               backgroundColor: MaterialStateProperty.all(Colors.blue),
//               textStyle: MaterialStateProperty.all(
//                 const TextStyle(color: Colors.white),
//               ),
//               minimumSize: MaterialStateProperty.all(
//                 Size(MediaQuery.of(context).size.width / 2.5, 50),
//               ),
//             ),
//             child: const Text(
//               "Sign Out",
//               style: TextStyle(color: Colors.white, fontSize: 16),
//             ),
//           ),
//         ],
//       ),
//     );
//   }
// }